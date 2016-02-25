// ******************************************************************************************* //
//
// File:         lab1p1.c
// Date:         
// Authors:      
//
// Description: Part 1 for lab 1
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "leds.h"
#include "interrupt.h"
#include "switch.h"
#include "timer.h"

typedef enum stateEnum {
    RUN, STOP
}state;

volatile state curr = STOP;
volatile state next;

/* Please note that the configuration file has changed from lab 0.
 * the oscillator is now of a different frequency.
 */
int main(void)
{
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    LEDEnable();
    
    LCD_Enable();
    
   
    
    
    TRISGbits.TRISG12 = 0;
    TRISGbits.TRISG15 = 0;
    LATGbits.LATG12 = 0;
    LATGbits.LATG15 = 1;
    
    ANSELEbits.ANSE7 = 0;
    CNPUEbits.CNPUE7 = 1;
    TRISEbits.TRISE7 = 1;
    CNCONEbits.ON = 1;
    CNENEbits.CNIEE7 = 1;
    IFS1bits.CNEIF = 0;
    IPC8bits.CNIP = 7;
    IEC1bits.CNEIE = 1;
    while(1){
        switch(curr) {
            case RUN:
                LATGbits.LATG12 = 0;
                LATGbits.LATG15 = 1;
                next = STOP;
                break;
            case STOP:
                LATGbits.LATG12 = 1;
                LATGbits.LATG15 = 0;
                next = RUN;
                break;
        }
    }
    
    return 0;
}

__ISR (_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void) {
    IFS1bits.CNEIF = 0;
    IFS1bits.CNDIF = 0;
    CNCONEbits.ON = 0;
    delayMs(5);
    CNCONEbits.ON = 1;
    if(PORTEbits.RE7 == 1) {
        curr = next;
    }
    if(PORTDbits.RD6 == 0) {
        LCD_Enable();
        LED_On(1);
    }
    if(PORTDbits.RD6 == 1) {
        LCD_Enable();
        LATDbits.LATD1 = 0;
    }
}