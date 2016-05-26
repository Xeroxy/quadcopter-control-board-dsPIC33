#include "main.h"
#include "common.h"

static const float ticksPerSecond = (float)FCY;
static const float ticksPerMs = (float)FCY/1000;
static const float ticksPerUs = (float)FCY/1000000;

unsigned long OverflowCount;

//Control loop timer
void setupTimer1(){
    IEC0bits.T1IE = 0; //Disable timer1 interrupt
    T1CONbits.TON = 0; //Disable timer
    T1CONbits.TSIDL = 0; //Continue operation in idle mode
    T1CONbits.TGATE = 0; //Timer gate accumulation disabled
    T1CONbits.TCKPS = 0b00; //Timer prescale 1:1, 1:8, 1:64, 1:256
    T1CONbits.TCS = 0; //Internal clock source //0
    IPC0bits.T1IP = 0b100; //Priority 4

    PR1 = 65535; //Period register 

    TMR1=0;
    T1CONbits.TON = 1; //Enable timer

    printf("\nTimer 1 - Setup Complete");
}
void resetTimer1(){
        T1CONbits.TON = 0;
        TMR1 = 0;            // Reset counter
        OverflowCount=0;
        T1CONbits.TON = 1;
}
/*
 * Returns the time in seconds since the last call
 * of this function (or last reset)
 */
float getDt(){
        unsigned long  ticks = TMR1;
        float ticksMs;
        ticks=ticks+(unsigned long)(OverflowCount*65535);
        ticksMs=(float)ticks/ticksPerMs;
        //printf(" Over %lu %lu %3.9fms",ticks,Overflow_count,ticks_ms);
        resetTimer1();
        //in Micro Seconds
        //printf(" Ticks %lu %1.3f",ticks,FCY);
        return  ticksMs;
}

void _ISR __attribute__((__interrupt__,no_auto_psv)) _T1Interrupt(void){
    IFS0bits.T1IF = 0; //Clear interrupt flag
    IEC0bits.T1IE = 0; //Disable timer1 interrupt
    LATAbits.LATA0 = !LATAbits.LATA0;
    OverflowCount++;

    //getAccelValues();
    //getGyroValues();
    //getMagValues();

    //getAccelRate();
    //getGyroRate();

    //getAccelAngles();
    //getGyroAngles();

    //SecondOrderComplementaryFilter();
    //computeAllPID();
    updateMotors();
    
    IEC0bits.T1IE = 1; //Enable timer1 interrupt
}

