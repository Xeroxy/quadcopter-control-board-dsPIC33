#include "main.h"
#include "common.h"

PIDData PIDD[LAST]={{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
PIDOutput PIDO={0.0,0.0,0.0};

void setupPID(){
    PIDD[ROLL].Kd=1;
    PIDD[ROLL].Ki=1;
    PIDD[ROLL].Kp=1;

    PIDD[PITCH].Kd=1;
    PIDD[PITCH].Ki=1;
    PIDD[PITCH].Kp=1;

    PIDD[YAW].Kd=1;
    PIDD[YAW].Ki=1;
    PIDD[YAW].Kp=1;
}
float computePID(float target, float input, PIDData *PIDC){

    float erreur=target-input;
    PIDC->iErr+=(PIDC->Ki*erreur);

    float dErr=(input-PIDC->lastInput);
    PIDC->lastInput=input;

    return (PIDC->Kp*erreur+PIDC->iErr-PIDC->Kd*dErr);
}

void computeAllPID(){
     PIDO.roll=computePID(ICO.rollTarget, CA.XAngle, &PIDD[ROLL]);
     PIDO.pitch=computePID(ICO.pitchTarget, CA.YAngle, &PIDD[PITCH]);
     PIDO.yaw=computePID(ICO.yawTarget, CA.ZAngle, &PIDD[YAW]);
}
