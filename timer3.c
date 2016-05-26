#include "main.h"
//Output pulse timer

void setupTimer3(){
    IEC0bits.T3IE = 0; //Disable timer2 interrupt
    T3CONbits.TON = 0; //Disable timer
    T3CONbits.TSIDL = 0; //Continue operation in idle mode
    T3CONbits.TGATE = 0; //Timer gate accumulation disabled
    T3CONbits.TCKPS = 0b01; //01//Timer prescale 1:1, 1:8, 1:64, 1:256
    T3CONbits.TCS = 0; //Internal clock source

    PR3 = 65535; //Period register

    TMR3=0;

    T3CONbits.TON = 1; //Disable timer
    printf("\nTimer 3 - Setup Complete");
}


void enableTimer3(){
    TMR3=0;
    T3CONbits.TON = 1;
}

void disableTimer3(){
    TMR3=0;
    T3CONbits.TON = 0;
}