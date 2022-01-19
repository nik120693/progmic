/*
 * Title: Timer.c
 * Description: Timer configuration function and Timer control
 * Author: CTa
 * Date: 09.10.2021
*/
#include <xc.h>
#include <p32xxxx.h>
#include "multivectormode_interrupt.h"



void Timer2_init(void)
{
    T2CONbits.ON = 0;   // Disable Timer2
    T2CONbits.T32 = 0;  // not use 32-bit mode - use 16-bit mode
    T2CONbits.TCKPS = 0b111; //select prescaler 256
    T2CONbits.TCS = 0;  //select internal peripheral clock
    TMR2 = 0;           //Clear TMR2 register
    PR2 = 39063;        // Set PR2 register - Calculated to have 0.5s - Change it for new delays (See also PBCLK due to #pragma)
    
    IPC2bits.T2IP = 1;
    IPC2bits.T2IS = 0;
    IFS0bits.T2IF = 0;
    
    
    macro_enable_interrupts();
    
    IEC0bits.T2IE = 1;
    T2CONbits.ON = 1;   // Enable Timer2
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