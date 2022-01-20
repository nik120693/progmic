/* 
 * File:   main.c
 * Authors: Samuele Guccione, Paolo Gariboldi, Massimo Giuttari, Francesco Nicastro
 * 
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "lcd.h"
#include "aic.h"
#include "adc.h"
#include "rgbled.h"
#include "uart.h"
#include "utils.h"
#include "btn.h"
#include <time.h>
#include "List.h"
#include "Timer.h"
#include "audio.h"

#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF      

// Device Config Bits in  DEVCFG1:	
#pragma config FNOSC =	FRCPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	XT
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_2

// Device Config Bits in  DEVCFG2:	
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1

void Sensor();
void Disattivato();
void Attivato();
void Menu();
void MenuAttivato();
void ChangePassword();
void Initstrg();
void Audio();
int trigger_EventLog();

int in = 0;
int mseconds = 0;

Node *logs;
char pwd[5] = {"0000"};
char flagRX = 0; // global  - it has to be init in main.c
char strg[80]; // global - it has to be init in main.c
char strgpassword[80];
int j = 0;
unsigned int baud = 9600;

int seconds = 0;
int timeReachedDelay = 0;

volatile unsigned char fRxDone;

void __attribute__((interrupt(ipl1), vector(8)))
    Timer2Handler(void) {
    seconds++;
    IFS0bits.T2IF = 0;
}

    void __attribute__((interrupt(ipl2), vector(16)))
    Timer4Handler(void) {
    timeReachedDelay = 1;
    IFS0bits.T4IF = 0;
}

int main(int argc, char** argv) {
    TRISBbits.TRISB14 = 0; //buzzer configured as output
    ANSELBbits.ANSB14 = 0; //buzzer disabled analog
    PORTBbits.RB14 = 0;
    logs = init_list();
    LCD_Init();
    LCD_DisplayClear();
    UART_ConfigurePins();
    UART_ConfigureUart(baud);
    BTN_Init();
    AIC_Init();

    in = 1;
    mseconds = 0;
       
    Timer2_init();
    Timer4_init();
    
    Disattivato();

    return (1);
}

void Disattivato() {
    RGBLED_Init();

    RGBLED_SetValue(255, 0, 0);
    LCD_WriteStringAtPos("Disattivato", 0, 0);

    while (1) {
        /*tasto BTNC per proseguire*/
        if (BTN_GetValue(2)) { //BTNC
            LCD_DisplayClear();
            RGBLED_Close();
            Attivato();
        }

        /*tasto BTND torna al menu*/
        if (BTN_GetValue(4)) {
            RGBLED_Close();
            Disattivato();
        }
    }
}

void Sensor() {
    char strMsg[80];
    int valPot;
    RGBLED_Init();

    LCD_DisplayClear();
    while (1) {
        RGBLED_SetValue(0, 0, 255); //led blue
        valPot = AIC_Val();
        sprintf(strMsg, "Pot:%04d", valPot);
        LCD_WriteStringAtPos(strMsg, 0, 0);
        LCD_WriteStringAtPos("AIC", 1, 0);

        if (AIC_Val() > 512) {
           AUDIO_Init(0);
            delay(2);
            AUDIO_Close();
            add_EventLog(&logs, seconds/2);
        }
        /*tasto BTND torna al menu*/
        if (BTN_GetValue(4)) {
            RGBLED_Close();
            MenuAttivato();
        }
    }
}

void mostraLog() {
    Node* current = logs;

    if (current == NULL || current->data == -1) {
        for (int i = 0; i < 300000; i++) {

        }
        putU4_string("Lista vuota\r");
        flagRX = 0;
        return;
    }
    for (int i = 0; i < 300000; i++) {

    }
    putU4_string("Logs: \r");
    flagRX = 0;
    int count = 1;
    while (current != NULL) {
        index_List(count, 0);
        for (int i = 0; i < 300000; i++) {

        }
        putU4_string(": ");
        flagRX = 0;
        index_List(current->data, 1);
        current = current->next;
        count++;
    }
}

void Attivato() {
    LCD_DisplayClear();

    putU4_string("\n Inserisci Password:"); //uart
    unsigned int finished = 1;

    while (finished) {
        getU4_stringR();
        if (flagRX) {
            if (strcmp(strg, pwd) == 0) {
                putU4_string("\n Password Corretta");
                flagRX = 0; //svuota buffer uart
                Menu();
            } else {
                putU4_string("\n Password errata");
                flagRX = 0;
                getU4();
                Disattivato();
            }
            finished = 0;
        }
    }
}

void ChangePassword() {
    LCD_DisplayClear();
    putU4_string("\n Inserisci nuova Password: ");
    flagRX = 0;
    getU4(); //per eliminare \n dal buffer
    unsigned char a;
    unsigned char b;
    unsigned char c;
    unsigned char d;
    a = getU4();
    b = getU4();
    c = getU4();
    d = getU4();
    unsigned char res[5] = {a, b, c, d, '\0'};

    unsigned char invio = getU4();

    if (invio == '\r') {

        if (strcmp(pwd, res) == 0) {
            putU4_string("\n Nuova Password equivalente a quella della configurazione attuale ");
            flagRX = 0;
            getU4();
            Attivato();
        } else {
            for (int i = 0; i<sizeof (res) / sizeof (res[0]); i++) {
                pwd[i] = res[i];
            }
            putU4_string("\n Password aggiornata !");
            flagRX = 0;
            getU4();
            Attivato();
        }
    }
}

void Menu() {
    LCD_DisplayClear();
    
    LCD_WriteStringAtPos(" Attivato \0", 0, 0);
    LCD_WriteStringAtPos(":) \0", 1, 0);
    
    putU4_string("\n 1-Attiva");
    putU4_string("\n 2-Cambia Password");
    putU4_string("\n 3-Vedi Log");
    putU4_string("\n 4-Cancella Log \n");

    flagRX = 0;
    getU4_string();

    unsigned char d = getU4();
    unsigned int finished = 1;

    while (finished) {

        unsigned char c = getU4();

        if (c == '\r') {

            if ('1' == d) {
                flagRX = 0;
                Sensor();
            } else if ('2' == d) {
                flagRX = 0;
                ChangePassword();
            } else if ('3' == d) {
                mostraLog();
                flagRX = 0;
            } else if ('4' == d) {
                rm_List(&logs);
                flagRX = 0;
            } else {
                putU4_string("\n comando non riconosciuto, disattivo");
                Disattivato(); //stampa comando non riconosciuto, torna a disattivato
                flagRX = 0;

            }
            flagRX = 0;
            finished = 0;
        }
    }
}

void MenuAttivato() {
    LCD_DisplayClear();
    putU4_string("\n 1-Disattiva");
    putU4_string("\n 2-Cambia Password");
    putU4_string("\n 3-Vedi Log");
    putU4_string("\n 4-Cancella Log \n");

    unsigned char uno = '1';
    unsigned char due = '2';
    unsigned char tre = '3';
    unsigned char quattro = '4';

    flagRX = 0;
    getU4_string();

    unsigned char d = getU4();
    unsigned int finished = 1;

    while (finished) {
        unsigned char c = getU4();
        if (c == '\r') {

            if (d == uno) {
                putU4_string("\n Disattivato ");
                flagRX = 0;
                getU4();
                Disattivato();
            } else if (d == due) {
                flagRX = 0;
                ChangePassword();
            } else if (d == tre) {
                mostraLog();
                flagRX = 0;
            } else if (d == quattro) {
                rm_List(&logs);
                flagRX = 0;
            } else {
                putU4_string("\n comando non riconosciuto, disattivo");
                Disattivato(); //stampa comando non riconosciuto, torna a disattivato
                flagRX = 0;
            }
            flagRX = 0;
            finished = 0;
        }
    }
}
