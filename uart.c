#include "main.h"

#define HDLC_ESC        0x7D    //Escape char
#define HDLC_SEP        0x7E    //Packet separator char
#define HDLC_XOR        0x20    //Xor char

int __C30_UART=1; //Printf to UART...

void setupUART(unsigned long int baud){
    PPSUnlock;
    RPINR18bits.U1RXR = 0b00011; //Tie RX to RP3
    TRISBbits.TRISB5 = inb;	 //Set RB5 as input
    RPOR1bits.RP2R = 0b00011;    //Tie TX to RP2
    TRISBbits.TRISB2 = outb;	 //Set RB2 as output
    PPSLock;

    U1MODEbits.UARTEN = 0;      // Bit15 TX, RX DISABLED, ENABLE at end of func
    U1MODEbits.USIDL = 0;       // Bit13 Continue in Idle
    U1MODEbits.IREN = 0;        // Bit12 No IR translation
    U1MODEbits.RTSMD = 0;       // Bit11 Simplex Mode
    U1MODEbits.UEN = 0;         // Bits8,9 TX, RX enabled, CTS, RTS not
    U1MODEbits.WAKE = 0;        // Bit7 No Wake up (since we don't sleep here)
    U1MODEbits.LPBACK = 0;      // Bit6 No Loop Back
    U1MODEbits.ABAUD = 0;       // Bit5 No Autobaud (would require sending '55')
    U1MODEbits.URXINV = 0;      // Bit4 IdleState = 1 (for dsPIC)
    U1MODEbits.BRGH = 0;        // Bit3 16 clocks per bit period
    U1MODEbits.PDSEL = 0b00;    // Bits1,2 8bit, no Parity
    U1MODEbits.STSEL = 0;       // Bit0 One Stop Bit

    // Load a value into Baud Rate Generator.
    // See section 17.3.1 of datasheet.
    U1BRG = (unsigned int)((FCY/(16 * baud)) - 1); // 43 80Mhz osc, 57600 Baud

    // Load all values in for U1STA SFR
    U1STAbits.UTXISEL1 = 0;     //Bit15 Int when Char is transferred (1/2 config!)
    U1STAbits.UTXINV = 0;       //Bit14 N/A, IRDA config
    U1STAbits.UTXISEL0 = 0;     //Bit13 Other half of Bit15
    U1STAbits.UTXBRK = 0;       //Bit11 Disabled
    U1STAbits.UTXEN = 0;        //Bit10 TX pins controlled by periph
    U1STAbits.UTXBF = 0;        //Bit9 *Read Only Bit*
    U1STAbits.TRMT = 0;         //Bit8 *Read Only bit*
    U1STAbits.URXISEL = 0;      //Bits6,7 Int. on character recieved
    U1STAbits.ADDEN = 0;        //Bit5 Address Detect Disabled
    U1STAbits.RIDLE = 0;        //Bit4 *Read Only Bit*
    U1STAbits.PERR = 0;         //Bit3 *Read Only Bit*
    U1STAbits.FERR = 0;         //Bit2 *Read Only Bit*
    U1STAbits.OERR = 0;         //Bit1 *Read Only Bit*
    U1STAbits.URXDA = 0;        //Bit0 *Read Only Bit*

    IPC7 = 0x4400;              //Mid Range Interrupt Priority level, no urgent reason

    IFS0bits.U1TXIF = 0; //Clear the Transmit Interrupt Flag
    IEC0bits.U1TXIE = 0; //Disable Transmit Interrupts
    IFS0bits.U1RXIF = 0; //Clear the Recieve Interrupt Flag
    IEC0bits.U1RXIE = 0; //Disable Recieve Interrupts

    U1MODEbits.UARTEN = 1; //Turn the peripheral on
    U1STAbits.UTXEN = 1;

    U1MODEbits.URXINV=0; 
    __delay_ms(50); //UART requires some delay (at least one bit time) before sending data$

   // printf("\nUART Setup Complete");
}

void transmitChar(char inputChar){ //Transmit single char

    while(U1STAbits.UTXBF == 1){}
    U1TXREG = inputChar;
}

char recieveChar(){
    while(U1STAbits.URXDA==0){} //Wait for new data
    return(U1RXREG);
}

void sendChar(char c){
    WriteUART1(c);
    while(BusyUART1());
}
void hdlcSendSep(){
    sendChar(HDLC_SEP);
}
void hdlcSendByte(unsigned char c){
    if(HDLC_SEP == c || HDLC_ESC == c){
            sendChar(HDLC_ESC);
            sendChar(c ^ HDLC_XOR);
    }else{
            sendChar(c);
    }
}
void hdlcSendWord(unsigned int w){
    //send Most Significat Byte First
    hdlcSendByte(w >> 8 );
    hdlcSendByte(w & 0xFF);
}
void hdlcSendFloat(float f){
    unsigned char* p = (unsigned char*)(&f);
    hdlcSendByte(*p);p++;
    hdlcSendByte(*p);p++;
    hdlcSendByte(*p);p++;
    hdlcSendByte(*p);
}