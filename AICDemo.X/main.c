/* 
 * File:   main.c
 * Author: Monica Ignat
 *
 * Created on March 30, 2017
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


#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF      


/* ------------------------------------------------------------ */
/*						Configuration Bits		                */
/* ------------------------------------------------------------ */


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

char pwd[5] = {"0000"};
//int pwd = 0000;
char flagRX; // global  - it has to be init in main.c
char strg[80]; // global - it has to be init in main.c
int j = 0;
unsigned int baud = 9600;

volatile unsigned char fRxDone;

int main(int argc, char** argv) 
{
    //AICDemo();      
    Disattivato();
    //Attivato();
    /*
    while(1) {
        
    }
    */
    //se btnc premuto -> attivato
    return (1);
}
/***	AICDemo
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the Analon Input control module and uses the ADC and AIC functions to read an analog pin
**          
*/
Sensor(){
    //libraries initialization for the used IO modules
    char strMsg[80];    
    int valPot;
    
    //LCD_Init(); 
    AIC_Init();
    RGBLED_SetValue(0,0,1); //led blue
    
    while(1)
    {    
        valPot = AIC_Val();
        sprintf(strMsg, "Pot:%04d", valPot);
        LCD_WriteStringAtPos(strMsg, 0, 0);
        LCD_WriteStringAtPos("AIC Demo", 1, 0);
        /*if(AIC_Val() > 512) {
            //suono
            LATBbits.LATB14 ^= 1; // non funziona e non so perchè
        }*/
        
    }    
}

Disattivato() {
    LCD_Init();
    RGBLED_Init();
    LCD_DisplayClear();
    //UART_Init(baud);
    //UART_InitPoll(baud);
    UART_ConfigurePins();
    UART_ConfigureUart(baud);
    
    BTN_Init();
    RGBLED_SetValue(1,0,0); 
    LCD_WriteStringAtPos("Disattivato", 0, 0);
    
    while(1) {
        /*legge da aic il valore del potenziometro*/
        if(BTN_GetValue(2)){ //BTNC
            RGBLED_Close();
            LCD_DisplayClear();
            //AICDemo();
            Attivato();
        }
        
        /*tasto BTNU torna al menu*/
        if(BTN_GetValue(4)) {
            Disattivato();
        }
    }
}



Attivato() {
    
    //LCD_Init();
    //LCD_DisplayClear();
    LCD_WriteStringAtPos("Inserisci Password\0", 0, 0);
    LCD_WriteStringAtPos("da terminale\0", 1, 0);
    
    //RGBLED_SetValue(1,0,0); //va messo nel while sennò il programma non prosegue
    
    /*getU4_string();
        if(flagRX)
        {
            if(strcmp(strg, "ledon")==0)
            {
                putU4_string("Accendi il led\r");
                TRISA = 0xBF00;
                LATA=0x0001;
            }

            flagRX = 0;
        }*/
    
    putU4_string("Inserisci Password:");
    //getU4_string();
    //flagRX = 0;

    while(1){
        getU4_string();
        if(flagRX)
        {
            if(strcmp(strg, pwd)==0)
            {
                putU4_string("Password Corretta\n");
                flagRX=0; //svuota buffer uart
                Menu();
            }
            else {
                putU4_string("Password errata\n"); 
                flagRX=0;
            }
            break;
        }
    }
    
    
    //passare strg a pwd
    //strcpy(pwd,strg); //nel change password
    DelayAprox10Us(10000);
 
    
}

void Audio(){
    LATBbits.LATB14 ^= 1;
}

Menu() {
    //da implementare
    putU4_string("1-Attiva\n");
    putU4_string("2-Cambia Password\n");
    putU4_string("3-Vedi Log\n");
    putU4_string("4-Cancella Log\n");
    char scelta[80];
    
    char uno[2] = "1";
    char due[2] = "2";
    char tre[2] = "3";
    char quattro[2] = "4";
    
    while(1){
        getU4_string();
        if(flagRX)
        {
            if(strcmp(scelta, uno)==0){
                Sensor();
                flagRX=0;
            }
            else if(strcmp(scelta, due)==0){
                //cambia password
                flagRX=0;
            }
            else if(strcmp(scelta, tre)==0) {
                //vedi log
                flagRX=0;
            }
            else if(strcmp(scelta, quattro)==0) {
                //cancella log
                flagRX=0;
            }
            else{
                putU4_string("comando non riconosciuto, disattivo");
                Disattivato(); //stampa comando non riconosciuto, torna a disattivato
                flagRX=0;
                
            }
            flagRX=0;
            break;
        }
    }
    
}
