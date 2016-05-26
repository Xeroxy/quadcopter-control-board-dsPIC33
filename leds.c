#include "main.h"

void setupLeds(){
    TRISAbits.TRISA0=outb;
    TRISAbits.TRISA1=outb;
}
void setLedsHigh(){
    LATAbits.LATA0=1;
    LATAbits.LATA1=1;
}
void setLedsLow(){
    LATAbits.LATA0=0;
    LATAbits.LATA0=0;
}