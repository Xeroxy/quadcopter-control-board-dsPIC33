#include "common.h"

#define timeConstant 0.1

float a = 0.9994;

float filter_xterm[3] = {0,0,0};
float filter_yterm[3] = {0,0,0};

ComplementaryAngles CA={0,0};

/*
void SecondOrderComplementaryFilter(){
   filter_xterm[0] = (AO.XAngle - CA.XAngle) * timeConstant * timeConstant;
   filter_yterm[0] = (AO.YAngle - CA.YAngle) * timeConstant * timeConstant;
   filter_xterm[2] = (dt * filter_xterm[0]) + filter_xterm[2];
   filter_yterm[2] = (dt * filter_yterm[0]) + filter_yterm[2];
   filter_xterm[1] = filter_xterm[2] + (AO.XAngle - CA.XAngle) * 2 * timeConstant + GO.XRate;
   filter_yterm[1] = filter_yterm[2] + (AO.YAngle - CA.YAngle) * 2 * timeConstant + GO.YRate;
   CA.XAngle = (dt * filter_xterm[1]) + CA.XAngle;
   CA.YAngle = (dt * filter_yterm[1]) + CA.YAngle;
}
*/