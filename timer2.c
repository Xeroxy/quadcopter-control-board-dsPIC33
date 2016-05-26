#include "main.h"
#include "common.h"

//Input capture timer
void setupTimer2(){
    IEC0bits.T2IE = 0; //Disable timer2 interrupt
    T2CONbits.TON = 0; //Disable timer
    T2CONbits.TSIDL = 0; //Continue operation in idle mode
    T2CONbits.TGATE = 0; //Timer gate accumulation disabled
    T2CONbits.TCKPS = 0b01; //Timer prescale 1:1, 1:8, 1:64, 1:256
    T2CONbits.T32 = 0; //32 bit timer disabled
    T2CONbits.TCS = 0; //Internal clock source



    //Reset manually
    PR2 = 65535; //Period register
    TMR2=0;

    T2CONbits.TON = 1; //Enable timer2
     
    printf("\nTimer 2 - Setup Complete");
}

void restartTimer2(){
    TMR2=0;
    T2CONbits.TON = 1;
}