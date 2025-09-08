
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
#define rs RC0
#define rw RC1
#define e RC2
#define _XTAL_FREQ 20000000



void lcd();
void dat(unsigned char a);
void cmd(unsigned char a);
void cmd(unsigned char a){
 PORTD =a;
rs=0;
rw=0;
e=1;
__delay_ms(100);
e=0;
}
void lcd(){
	
 cmd(0X38);
 cmd(0X0e);
	cmd(0X01);
	cmd(0X06);
	cmd(0X0c);
	cmd(0X80);
}

void dat(unsigned char a){
 PORTD = a;
rs=1;
rw=0;
e=1;
__delay_ms(100);
e=0;
}


void main(){
    TRISC = 0x00;
    TRISD = 0x00;
   	lcd();
	
	while(1){
        
		dat('!');
		__delay_ms(100);
        dat('A');
        dat('a');
        dat('d');
        __delay_ms(100);
		
	}
    
}