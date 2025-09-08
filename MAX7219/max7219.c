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
#define load RC0 

void max_write(char add,char dat){
    load =1;
    __delay_ms(100);
    load = 0;
    SSPBUF = add;
    while(SSPIF == 0);
    SSPIF = 0;
    SSPBUF = dat;
    while(SSPIF == 0);
    SSPIF = 0;
    load = 1;
    
}

void main(){
    TRISC = 0X00;
    SSPSTAT = 0X40;
    SSPCON = 0X20;
    max_write(0X0C,0X01);
    max_write(0X0F,0X00);
    max_write(0X09,0XFF);
    max_write(0X0B,0X07);
    max_write(0X0A,0X08);
    
    
        max_write(0X01,0);
        max_write(0X02,1);
        max_write(0X03,4);
        max_write(0X04,1);
        max_write(0X05,3);
        max_write(0X06,0);
        max_write(0X07,0);
        max_write(0X08,0);
        while(1){
    }
    
    
    
}
