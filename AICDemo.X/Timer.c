/*
 * Title: Timer.c
 * Description: Timer configuration function and Timer control
 * Author: CTa
 * Date: 09.10.2021
*/
#include <xc.h>
#include <p32xxxx.h>
#include "config.h"
//#include "Timer.h"



void Timer2_init(void)
{
    T2CONbits.ON = 0;   // Disable Timer2
    T2CONbits.T32 = 0;  // not use 32-bit mode - use 16-bit mode
    T2CONbits.TCKPS = 0b111; //select prescaler 256
    T2CONbits.TCS = 0;  //select internal peripheral clock
    TMR2 = 0;           //Clear TMR2 register
    //PR2 = 39063;        // Set PR2 register - Calculated to have 0.5s - Change it for new delays (See also PBCLK due to #pragma)
    PR2 = 390630; // 5 secondi
    IPC2bits.T2IP = 1;
    IPC2bits.T2IS = 0;
    IFS0bits.T2IF = 0;
    
    
    macro_enable_interrupts();
    
    IEC0bits.T2IE = 1;
    T2CONbits.ON = 1;   // Enable Timer2 , era ad 1
}

void turning_on_with_timer()
{ 
    // -> turn on LED0
	
	/* delay with timer */
    TMR2 = 0;
    while(TMR2 < PR2-1);
	
   // -> turn on LED1
   
   /* delay with timer */
    TMR2 = 0;
    while(TMR2 < PR2-1);
    
	/* to continue with next LEDs*/
    
}


void turning_off_with_timer()
{
    /* the same as tunrnig_on function but put LEDs in OFF state */
}

extern int timeReachedDelay;

void Timer4_init(void) {
    T4CONbits.ON = 0;   // Disable Timer 4
    T4CONbits.TCKPS = 0b111; //select prescaler 256
    T4CONbits.TCS = 0;  //select internal peripheral clock
    TMR4 = 0;           // Clear TMR4 register
    
    //impostazione intterrup sul timer 4
    IPC4bits.T4IP = 2; //priorità 1 (1-7)
    IPC4bits.T4IS = 0; //subpriorità 1 (1-4)
    IFS0bits.T4IF = 0; //Timer 4 interrupt Flag

    macro_enable_interrupts(); //multi vector mode function (del prof.)
    IEC0bits.T4IE = 1; //Timer4 interrupt enabled
    
    PR4 = 39063;        // Set PR4 register - Calculated to have 0.5s - Change it for new delays (See also PBCLK due to #pragma)
    T4CONbits.ON = 1;   // Enable Timer 4
}

void wait() {
    TMR4 = 0;   
    while (!timeReachedDelay); 
    timeReachedDelay = 0;
}

void delay(int seconds){
    for(int i = 0; i < seconds*2; i++){
        wait();
    }
}
