// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 20000000


void main()
{
    TRISC = 0X00;
    TRISC3 = 1;
    TRISC4 = 1;
    
	SSPCON=0x28;
	SSPADD=0X3F;
	
	SEN=1;
	while(SSPIF==0);
	SSPIF=0;
	
	SSPBUF=0XA0;
	while(SSPIF==0);
	SSPIF=0;
	
	SSPBUF=0X00;
	while(SSPIF==0);
	SSPIF=0;
	
	SSPBUF=0X00;
	while(SSPIF==0);
	SSPIF=0;

	SSPBUF='A';
	while(SSPIF==0);
	SSPIF=0;
    
    __delay_ms(10);

	PEN=1;
	while(SSPIF==0);
	SSPIF=0;
	
	while(1);
	
}