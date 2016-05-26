#include"main.h"

void setupPWM(){
    T2CONbits.TON = 0; //Disable timer

    PPSUnlock;
    RPOR7bits.RP15R = 0b10101; //Tie OC4 to RP14
    RPOR4bits.RP8R = 0b10010;  //Tie OC1 to RP8 //Page 167
    RPOR5bits.RP10R = 0b10011; //Tie OC2 to RP10
    RPOR6bits.RP12R = 0b10100; //Tie OC3 to RP12
    PPSLock;

    IEC1bits.OC4IE = 0; //Disable OC4 interrupt
    IEC0bits.OC1IE = 0; //Disable OC1 interrupt
    IEC0bits.OC2IE = 0; //Disable OC2 interrupt
    IEC1bits.OC3IE = 0; //Disable OC3 interrupt
    
     // Pins des moteurs en out
    TRISBbits.TRISB15=outb; //PWM1H1
    TRISBbits.TRISB12=outb; //PWM1H2
    TRISBbits.TRISB10=outb; //PWM1H3
    TRISBbits.TRISB8=outb; //PWM2H1

    OC4CONbits.OCSIDL = 0; //OC continues in idle mode
    OC4CONbits.OCFLT = 0;  //Fault status bit, N/A as fault pin is disabled
    OC4CONbits.OCTSEL = 1; //Timer2 source
    OC4CONbits.OCM = 0b110;//PWM mode, no fault pin

    OC1CONbits.OCSIDL = 0; //OC continues in idle mode
    OC1CONbits.OCFLT = 0;  //Fault status bit, N/A as fault pin is disabled
    OC1CONbits.OCTSEL = 1; //Timer2 source
    OC1CONbits.OCM = 0b110;//PWM mode, no fault pin

    OC2CONbits.OCSIDL = 0; //OC continues in idle mode
    OC2CONbits.OCFLT = 0;  //Fault status bit, N/A as fault pin is disabled
    OC2CONbits.OCTSEL = 1; //Timer2 source
    OC2CONbits.OCM = 0b110;//PWM mode, no fault pin

    OC3CONbits.OCSIDL = 0; //OC continues in idle mode
    OC3CONbits.OCFLT = 0;  //Fault status bit, N/A as fault pin is disabled
    OC3CONbits.OCTSEL = 1; //Timer2 source
    OC3CONbits.OCM = 0b110;//PWM mode, no fault pin

    OC4RS = 62800;
    OC1RS = 62800; //OC secondary register - 16bits
    OC2RS = 62800;
    OC3RS = 62800;
    
    TMR2=0; //Clear timer 2
    T2CONbits.TON = 1; //Enable timer

    printf("\nPWM Setup Complete");

}