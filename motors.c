#include "main.h"
#include "common.h"

#define MotorMin 3245
#define MotorMax 5825
#define MotorRange 2580
#define K 0.7071

OCOutput OCO={0.0,0.0,0.0,0.0};

int floatToInt(float v){
        if(v>=0){
                return(int)(v+0.5f);
        }
        else{
                return(int)(v-0.5f);
        }
}

void updateMotors(){
        /*
        OCO.OC1Output=ICD.throttleInput;
        OCO.OC2Output=ICD.throttleInput;
        OCO.OC3Output=ICD.throttleInput;
        OCO.OC4Output=ICD.throttleInput;
        */
    /*
        OCO.OC1Output=MotorMin+MotorRange*ICO.throttle-ICO.pitchTarget+ICO.rollTarget+ICO.yawTarget;
        OCO.OC2Output=MotorMin+MotorRange*ICO.throttle+ICO.pitchTarget+ICO.rollTarget-ICO.yawTarget;
        OCO.OC3Output=MotorMin+MotorRange*ICO.throttle+ICO.pitchTarget-ICO.rollTarget+ICO.yawTarget;
        OCO.OC4Output=MotorMin+MotorRange*ICO.throttle-ICO.pitchTarget-ICO.rollTarget-ICO.yawTarget;
     */
        
        OCO.OC1Output=MotorMin+MotorRange*ICO.throttle;
        OCO.OC2Output=MotorMin+MotorRange*ICO.throttle;
        OCO.OC3Output=MotorMin+MotorRange*ICO.throttle;
        OCO.OC4Output=MotorMin+MotorRange*ICO.throttle;
         
        /*
        OCO.OC1Output=K*PIDO.roll-K*PIDO.pitch+PIDO.yaw+MotorMin+MotorRange*ICO.throttle;
        OCO.OC2Output=K*PIDO.roll+K*PIDO.pitch-PIDO.yaw+MotorMin+MotorRange*ICO.throttle;
        OCO.OC3Output=-K*PIDO.roll+K*PIDO.pitch+PIDO.yaw+MotorMin+MotorRange*ICO.throttle;
        OCO.OC4Output=-K*PIDO.roll-K*PIDO.pitch-PIDO.yaw+MotorMin+MotorRange*ICO.throttle;*/

        if(OCO.OC1Output > MotorMax) {OCO.OC1Output = MotorMax;}
        if(OCO.OC2Output > MotorMax) {OCO.OC2Output = MotorMax;}
        if(OCO.OC3Output > MotorMax) {OCO.OC3Output = MotorMax;}
        if(OCO.OC4Output > MotorMax) {OCO.OC4Output = MotorMax;}

        if(OCO.OC1Output < MotorMin) {OCO.OC1Output = MotorMin;}
        if(OCO.OC2Output < MotorMin) {OCO.OC2Output = MotorMin;}
        if(OCO.OC3Output < MotorMin) {OCO.OC3Output = MotorMin;}
        if(OCO.OC4Output < MotorMin) {OCO.OC4Output = MotorMin;}

        OC1RS = ~(unsigned int)(OCO.OC1Output);
        OC2RS = ~(unsigned int)(OCO.OC2Output);
        OC3RS = ~(unsigned int)(OCO.OC3Output);
        OC4RS = ~(unsigned int)(OCO.OC4Output);
        //printf("\n***OC1RS : %d", OC1RS);
}

void calibrateESCEndpoints(){
    int x = 0;
    for (x=0; x<400; x++){
    OC1RS = ~MotorMax;
    OC2RS = ~MotorMax;
    OC3RS = ~MotorMax;
    OC4RS = ~MotorMax;
    __delay_ms(2.5);
    LATAbits.LATA0 = !LATAbits.LATA0;
    }

    for (x=0; x<400; x++){
    OC1RS = ~MotorMin;
    OC2RS = ~MotorMin;
    OC3RS = ~MotorMin;
    OC4RS = ~MotorMin;
    __delay_ms(2.5);
    LATAbits.LATA0 = !LATAbits.LATA0;
    }
printf("\nESC endpoints calibrated");
}
