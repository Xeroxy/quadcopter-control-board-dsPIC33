/* 
 * File:   setup.c
 * Author: Pierre
 *
 * Created on March 18, 2014, 2:10 PM
 */
#include "main.h"

void setupOsc(){
    // Configuration de l'oscillator à 80Mhz avec un quartz de 12 Mhz
        //Fosc=Fin*M/(N1*N2)=12*40/(3*2)=80Mhz, Fcy=Fosc/2=80Mhz/2=40Mhz=40MIPS
    PLLFBD=38;                      //M=40 -> M=PLLDIV+2
    CLKDIVbits.PLLPOST=0;           //N1=2
    CLKDIVbits.PLLPRE=1;          //N2=3
//        //Disable Watch Dog Timer
    RCONbits.SWDTEN=0;
    while(!OSCCONbits.LOCK);    //Wait for PLL ready
    __delay_ms(10);             //Wait for clock to stabilize
}
