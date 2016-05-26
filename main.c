/* 
 * File:   main.c
 * Author: Pierre
 *
 * Created on March 18, 2014, 2:01 PM
 */
#include "main.h"
#include "config.h"

int error=1;

 int main(void) {
     ADPCFG = 0xFFFF;				//make ADC pins all digital ??
     setupOsc();
     setupUART(57600);
     printf("\n------------------------------------------------------------------------------------\n************************    dsPIC Quadcopter Controller    ************************\n------------------------------------------------------------------------------------");

     setupI2C(400000);
          MPU9150TestI2C();
     do{
     setupIMU();
     error=MPU9150CheckRegisters();
     }while(error!=0);
     setupPWM();
     setupLeds();
     setLedsHigh();
 
     setupTimer1(); //Main timer
     setupTimer2(); //Input capture timer
     setupTimer3(); //PWM timer

     //calibrateGyros();
     //calibrateAccel();
    // calibrateMag();
     
     setupPID();

     setupIC();
     setupTimer4(); //RX timeout timer


     //calibrateESCEndpoints();

     IEC0bits.T1IE = 1;
     

    while(1){
     }






    return (EXIT_SUCCESS);
}