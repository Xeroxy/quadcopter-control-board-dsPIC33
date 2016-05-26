#include "main.h"
#include "I2C.h"

void setupI2C(long unsigned int i2cfreq){

 TRISBbits.TRISB5=0;
 TRISBbits.TRISB6=0;
 LATBbits.LATB5=0;
 LATBbits.LATB6=0;
    
    //This function will initialize the I2C(1) peripheral.
    //Now we will initialise the I2C peripheral for Master Mode, No Slew Rate
    //Control, SMbus levels, and leave the peripheral switched off.


        I2C1CONbits.I2CEN = 0;			// Enables the I2C module and configures the SDA and SCL pins as serial port pins
	I2C1CONbits.I2CSIDL = 0;		// Continue module operation in idle mode
	I2C1CONbits.SCLREL = 1;			// Release the clock
	I2C1CONbits.IPMIEN = 0;			// Only acknowledge own address
	I2C1CONbits.A10M = 0;			// 7bit slave address
	I2C1CONbits.DISSLW = 1;			// 1=Slew rate control disabled (enable for 400kHz operation!)
	I2C1CONbits.SMEN = 0;			// Disable SMBus Input thresholds (set for 3.3V operation!)
	I2C1CONbits.GCEN = 0;			// Enable interrupt on a general address call
	I2C1CONbits.STREN = 0;			// Enable software or receive clock stretching (important when dealing with interrupts)
	I2C1CONbits.ACKDT = 0;			// Send ACK during acknowledge
	I2C1CONbits.ACKEN = 0;			// Acknowledge sequence not in progress
	I2C1CONbits.RCEN = 0;			// Receive sequence not in progress
	I2C1CONbits.PEN = 0;			// STOP condition not in progress
	I2C1CONbits.RSEN = 0;			// Repeated START condition not in progress
	I2C1CONbits.SEN = 0;


    //Clearing the recieve and transmit buffers
    I2C1RCV = 0x0000;
    I2C1TRN = 0x0000;

        /*Set I2C Baud Rate */
    /*Note: from dsPIC33F Family Reference Manual: I2C, section 19.4.3
Baud rate is set according to the following equation:
I2CBRG = (FCY/FSCL - FCY/10000000)-1 */
    I2C1BRG = (FCY/i2cfreq)-(FCY/10000000)-1;

    //Now we can enable the peripheral
    I2C1CONbits.I2CEN = 1;

    /*  IPC4bits.MI2C1IP = 5;			// set the master interrupt priority
        I2C1STATbits.I2COV = 0;          	// clear Overflow flag
        IPC4bits.MI2C1IP = 2;                   // set the master interrupt priority
        I2C1STATbits.I2COV = 0;                 // clear Overflow flag
        IEC1bits.SI2C1IE = 0;			// disables slave int
	I2C1CONbits.I2CEN = 1;			// Enables the I2C module and configures the SDA and SCL pins as serial port pins*/

    __delay_ms(10);

    printf("\nI2C Setup Complete");
}

unsigned char ByteReadI2C(unsigned char SlaveAddress, unsigned char SlaveRegister) {
    unsigned char Data;

    /*begin start condition */
    IdleI2C1(); //wait for bus to be idle
    IFS1bits.MI2C1IF = 0; //clear interrupt flag
    I2C1CONbits.SEN = 1; //set start event
    while(I2C1CONbits.SEN); //wait for the startup sequence to complete

    /*send device address (r/w cleared) */
    I2C1TRN = SlaveAddress;
    IFS1bits.MI2C1IF = 0;
    while(I2C1STATbits.TBF); //wait for data to clock out
    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
    IdleI2C1();

    /*send slave register address*/
    I2C1TRN = SlaveRegister; //status register of RTC
    IFS1bits.MI2C1IF = 0;
    while(I2C1STATbits.TBF); //wait for data to clock out
    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
    IdleI2C1();

    /*initiate a repeated start*/
    //__delay_us(100);
    while(I2C1CON & 0x001F); //wait for the module to be ready (see 19.5.6 of dsPIC33 reference manual 12C)
    I2C1CONbits.RSEN = 1;
    while(I2C1CONbits.RSEN); //wait for slave to respond
    //__delay_us(100);

    /*send device address (read/write set) */
    I2C1TRN = SlaveAddress | 0x1;
    IFS1bits.MI2C1IF = 0;
    while(I2C1STATbits.TBF); //wait for data to clock out
    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
    IdleI2C1();

    /*receive data from device*/
    I2C1CONbits.RCEN = 1; //receive enable (start clocking for slave transfer)
    while(I2C1CONbits.RCEN); //wait for data (NOTE: can also consider polling RBF bit here)
    Data = I2C1RCV;
    IdleI2C1();

    NotAckI2C1();
    while(I2C1CONbits.ACKEN);

    /*generate stop bus event*/
    IFS1bits.MI2C1IF = 0;
    while(I2C1CON & 0x001F); //wait for the module to be ready (see 19.5.5 of dsPIC33 reference manual 12C)
    I2C1CONbits.PEN = 1;
    while(IFS1bits.MI2C1IF == 0);

    /*done!*/
    return Data;
}

void ByteWriteI2C(unsigned char SlaveAddress, unsigned char SlaveRegister, unsigned char Data){
   /*begin start condition */
    //while(I2C1STATbits.P == 1); //make sure the bus is idle
    IdleI2C1();
    IFS1bits.MI2C1IF = 0; //clear interrupt flag
    I2C1CONbits.SEN = 1; //set start event
    while(IFS1bits.MI2C1IF == 0); //wait for interrupt flag to signify end of start condition

    /*send device address (r/w cleared) */
    I2C1TRN = SlaveAddress;
    IFS1bits.MI2C1IF = 0;
    while(I2C1STATbits.TBF); //wait for data to clock out
    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
    IdleI2C1();

    /*send slave register address*/
    I2C1TRN = SlaveRegister; //status register of RTC
    IFS1bits.MI2C1IF = 0;
    while(I2C1STATbits.TBF); //wait for data to clock out
    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
    IdleI2C1();

    /*send data */
    I2C1TRN = Data; //status register of RTC
    IFS1bits.MI2C1IF = 0;
    while(I2C1STATbits.TBF); //wait for data to clock out
    while(I2C1STATbits.ACKSTAT); //wait for device to acknowledge
    IdleI2C1();

    /*generate stop bus event*/
    IFS1bits.MI2C1IF = 0;
    while(I2C1CON & 0x001F); //wait for the module to be ready (see 19.5.5 of dsPIC33 reference manual 12C)
    I2C1CONbits.PEN = 1;
    while(IFS1bits.MI2C1IF == 0);

    /*done!*/
}
