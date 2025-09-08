
// PIC16F877A Configuration Bit Settings

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
#define rs RB1
#define rw RB2
#define e RB3


void lcd();
void dat(unsigned char a);
void cmd(unsigned char a);
void display_number(unsigned int num);


void __interrupt() ccpm(){
    unsigned int value,initial;
    if(CCP1IF){
    CCP1IF=0;
        value=CCPR1H;
        initial=CCPR1L|value<<8;
        display_number(initial);
        
    }
        
}

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

void display_number(unsigned int num) {
    if (num == 0) {
        dat('0');
        return;
    }
    char buffer[10];
    int i = 0;
    while (num > 0) {
        buffer[i++] = (num % 10)+'0';
        num /= 10;
    }
    for (int j = i - 1; j >= 0; j--) {
        dat(buffer[j]);
    }
}
void dat(unsigned char a) {
    PORTD = a;
    rs = 1;
    rw = 0;
    e = 1;
    __delay_ms(100); // Reduced delay
    e = 0;
}


int main(){
    INTCON = 0X90;
    PIE1 = 0X04;
    TRISD = 0x00;
    TRISB0 = 1;
    TRISB1 = 0;
    TRISB2 = 0;
    TRISB3 = 0;
    TRISC2 = 1;
    TRISC3 = 0;
   	lcd();
    
    T1CON=0X01;
	CCP1CON=0X05;
	while(1){
     
    }
}