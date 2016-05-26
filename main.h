/* 
 * File:   main.h
 * Author: Pierre
 *
 * Created on March 18, 2014, 2:02 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <p33FJ128MC802.h>
#include <libpic30.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <delay.h>
#include <pwm.h>
#include <timer.h>
#include <ports.h>
#include <uart.h>
#include <math.h>
#include <dsp.h>
#include <i2c.h>

#include "inputCapture.h"
#include "MPU9150.h"
#include "I2C.h"
#include "I2CControleLib.h"


#define out 0x00
#define in 0xFF
#define outb 0
#define inb 1

#define OSC_FREQ 80000000 // 79.2275 Mhz
#define FCY (OSC_FREQ / 2)

/****** GENERAL CONFIG ******/
#define MICROSEC 40uL // # of cycles for 1uSec delay (minumum is 11 cycles)
#define MILLISEC (1000*MICROSEC) // # of cycles for 1mSec delay (minumum is 11 cycles)



#define _1MICROSEC MICROSEC
#define _3MICROSEC (3*MICROSEC)
#define _10MICROSEC (10*MICROSEC)
#define _20MICROSEC (20*MICROSEC)
#define _30MICROSEC (30*MICROSEC)
#define _100MICROSEC (100*MICROSEC)
#define _200MICROSEC (200*MICROSEC)
#define _300MICROSEC (300*MICROSEC)
#define _500MICROSEC (500*MICROSEC)
#define _1MILLISEC MILLISEC
#define _10MILLISEC (10*MILLISEC)
#define _20MILLISEC (20*MILLISEC)
#define _25MILLISEC (25*MILLISEC)
#define _50MILLISEC (50*MILLISEC)
#define _100MILLISEC (100*MILLISEC)
#define _200MILLISEC (200*MILLISEC)
#define _500MILLISEC (500*MILLISEC)



/*
 __delay_ms that is defined in libpic30.h overflows if d argument is greater than  2^32 / FCY
We'll define and use our own delay macros.
We can do this without loosing precision,
as long as our FCY is multiple of 1000 and 1000000.
These functions will have a better range:
delayMs -> d <= 2^32 / (FCY / 1000) ~ 107374 (for 40Mhz Fcy)
delayUs -> d <= 2^32 / (FCY / 1000000) ~ 107,374,182 (for 40Mhz Fcy)
*/

#define __delay_ms(d) __delay32( (unsigned long) (d)*(FCY/1000))
#define __delay_us(d) __delay32( (unsigned long) (d)*(FCY/1000000))


void setupTimer1(void); 
void resetTimer1(void);
float getDt(void);

void setupTimer2(void);
void restartTimer2(void);

void setupTimer3(void);
void enableTimer3(void);
void disableTimer3(void);

void setupTimer4(void);
void restartTimer4();

void setupOsc(void);

void setupOC(void);
void outputCompare(void);

void setupPWM(void);

void setupIC(void);

int floatToInt(float);
void updateMotors(void);
void calibrateESCEndpoints(void);

void setupUART(unsigned long int);
void transmitChar(char);
char recieveChar(void);

void setupLeds(void);
void setLedsHigh(void);
void setLedsLow(void);

void SecondOrderComplementaryFilter(void);

void setupI2C(long unsigned int);
unsigned char ByteReadI2C(unsigned char SlaveAddress, unsigned char SlaveRegister);
void ByteWriteI2C(unsigned char SlaveAddress, unsigned char SlaveRegister, unsigned char Data);

short isSwitched(float previousError, float currentError);

void setupPID(void);
//float computePID(float, float, PIDData*);
void computeAllPID(void);

#endif	/* MAIN_H */

