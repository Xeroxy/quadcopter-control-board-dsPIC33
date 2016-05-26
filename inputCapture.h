/* 
 * File:   inputCpature.h
 * Author: Pierre
 *
 * Created on March 18, 2014, 2:31 PM
 */

#ifndef INPUTCPATURE_H
#define	INPUTCPATURE_H

#define PPSUnlock __builtin_write_OSCCONL(OSCCON & 0xbf)
#define PPSLock __builtin_write_OSCCONL(OSCCON | 0x40)

//These values are specific to my hobbyking 6ch reciever
//CH3
#define ThrottleMax 5825.0
#define ThrottleMin 3245.0
//CH4
#define YawMax 5813.0
#define YawMid 4542.0
#define YawMin 3245.0
//CH2
#define PitchMax 5825.0
#define PitchMid 4550.0
#define PitchMin 3253.0
//CH1
#define RollMax 5812.0
#define RollMid 4547.0
#define RollMin 3231.0

//The desired quad angle range, 40 = 20 degrees either way
#define RollAngleRange 40.0
#define PitchAngleRange 40.0
#define YawRateRange 180.0


#endif	/* INPUTCPATURE_H */

