#include "main.h"
#include "common.h"

InputCaptureData ICD={0,0,0,0};
InputCaptureOutput ICO={0.0,0.0,0.0,0.0};


int trash=0;

void setupIC(){
    //Port definition and link RPx to ICx
    PPSUnlock;
    RPINR7bits.IC1R = 0b01001;  //Tie IC1 to RP09 -> Channel 1
    RPINR7bits.IC2R = 0b01011;  //Tie IC2 to RP11 -> Channel 2
    RPINR10bits.IC7R = 0b01101; //Tie IC7 to RP13 -> Channel 3
    RPINR10bits.IC8R = 0b01110; //Tie IC8 to RP14 -> Channel 4
    PPSLock;
    
    TRISBbits.TRISB9 = inb;  //RP9 en output
    TRISBbits.TRISB11 = inb; //RP11 en output
    TRISBbits.TRISB13 = inb; //RP13 en output
    TRISBbits.TRISB14 = inb; //RP14 en output

    //Configure ICx module using the IC1CON register.
/*ICSIDL controls the modules operation in idle mode. By clearing this bit the module will keep running.
ICTMR selects which timer module is to be used, with clearing the bit equalling timer3, and setting the bit equalling timer3. Timers 1 4 and 5 cannot be used with this module.
ICI controls how many capture events are required to cause an interrupt. We set this to 0 to interrupt on every capture. A value of 01 captures every 2nd event, 10 every 3rd, and 11 every 4th.
ICM is the most important register, as it controls what type of event will trigger a capture. From the microchip input capture manual:*/
        //IC1 - Channel 1
        IC1CONbits.ICSIDL = 0;  //Continue in idle mode
        IC1CONbits.ICTMR = 0;   //Use Timer2
        IC1CONbits.ICI = 0b00;  //Interrupt on every capture
        IC1CONbits.ICM = 0b001; //Every edge
        //IC2 - Channel 2
        IC2CONbits.ICSIDL = 0;  //Continue in idle mode
        IC2CONbits.ICTMR = 0;   //Use Timer2
        IC2CONbits.ICI = 0b00;  //Interrupt on every capture
        IC2CONbits.ICM = 0b001; //Every edge
        //IC7 - Channel 3
        IC7CONbits.ICSIDL = 0;  //Continue in idle mode
        IC7CONbits.ICTMR = 0;   //Use Timer2
        IC7CONbits.ICI = 0b00;  //Interrupt on every capture
        IC7CONbits.ICM = 0b001; //Every edge
        //IC8 - Channel 4
        IC8CONbits.ICSIDL = 0;  //Continue in idle mode
        IC8CONbits.ICTMR = 0;   //Use Timer2
        IC8CONbits.ICI = 0b00;  //Interrupt on every capture
        IC8CONbits.ICM = 0b001; //Every edge

        //Configure the related interrupt registers
        IEC0bits.IC1IE = 1;     //Enable interrupt
        IPC0bits.IC1IP = 0b110; //Priority 6
        IEC0bits.IC2IE = 1;     //Enable interrupt
        IPC1bits.IC2IP = 0b110; //Priority 6
        IEC1bits.IC7IE = 1;     //Enable interrupt
        IPC5bits.IC7IP = 0b110; //Priority 6
        IEC1bits.IC8IE = 1;     //Enable interrupt
        IPC5bits.IC8IP = 0b110; //Priority 6

     T2CONbits.TON = 1;
     ICD.throttleInput=0;

     printf("\nInput Capture Setup Complete");
}

void _ISR __attribute__((__interrupt__,no_auto_psv)) _IC1Interrupt(void){
   if(PORTBbits.RB9==1){
       restartTimer2();
       trash=IC1BUF;
   }
   else if(PORTBbits.RB9==0){
       ICD.rollInput=(float)ReadTimer2();
       ICO.rollTarget=RollAngleRange*((ICD.rollInput-RollMid)/(RollMax-RollMin));//<40*1
   }
   restartTimer4();
   IFS0bits.IC1IF=0; //Clear flag
}

void _ISR __attribute__((__interrupt__,no_auto_psv)) _IC2Interrupt(void){
    if(PORTBbits.RB11==1){
        restartTimer2();
        IC2BUF=trash;
    }
    else if(PORTBbits.RB11==0){
        ICD.pitchInput=(float)ReadTimer2();
        ICO.pitchTarget=PitchAngleRange*((ICD.pitchInput-PitchMid)/(PitchMax-PitchMin));//<40*1
    }
    restartTimer4();
    IFS0bits.IC2IF = 0; //Clear IC2 interrupt flag
}
void  _ISR __attribute__((__interrupt__,no_auto_psv)) _IC7Interrupt(void){
    if(PORTBbits.RB13==1){
        restartTimer2();
        trash=IC7BUF;
    }
    
    else if(PORTBbits.RB13==0){
        ICD.throttleInput=((float)ReadTimer2());
        ICO.throttle=((ICD.throttleInput-ThrottleMin)/(ThrottleMax-ThrottleMin)); //<1
    }
    restartTimer4();
    IFS1bits.IC7IF=0; //Clear IC7 interrupt flag*/
}
void _ISR __attribute__((__interrupt__,no_auto_psv)) _IC8Interrupt(void){
    if(PORTBbits.RB14==1){
        restartTimer2();
        trash=IC8BUF;
    }
    else if(PORTBbits.RB14==0){
        ICD.yawInput=(float)ReadTimer2();
        ICO.yawTarget=YawRateRange*((ICD.yawInput-YawMid)/(YawMax-YawMin));//k<1*180
    }
    restartTimer4();
    IFS1bits.IC8IF=0; //Clear IC8 interrupt flag
}

