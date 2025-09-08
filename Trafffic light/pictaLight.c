
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
char ca[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
int i,j;

void red(){
    for(i=0;i<3;i++){
           
            for(j=0;j<3;j++){
             RA0 =0;
                RA1 = 1;
            PORTB = ca[i];
            __delay_ms(50);
            RA0=1;
            RE0=0;
            PORTB = ca[j];
            __delay_ms(500);
            }
    
    }
}
void yellow(){
    for(i=0;i<1;i++){
           
            for(j=0;j<3;j++){
             RA0 =0;
                RA1 = 1;
            PORTB = ca[i];
            __delay_ms(50);
            RA0=1;
            RC0=0;
            PORTB = ca[j];
            __delay_ms(500);
            }
    }
}

void main(){
    TRISB = 0X00;
    TRISC = 0X00;
    TRISD = 0X00;
    TRISA = 0;
    TRISE = 0;
    PORTB = 0X00;
    PORTC = 0x00;
    
    while(1){
          PORTC = 0X18;
        PORTD = 0X09;
        red();
        PORTC = 0X14;
        PORTD = 0X09;
        yellow();
        PORTC = 0X42;
        PORTD = 0X09;
        red();
        PORTC = 0X22;
        PORTD = 0X09;
        yellow();
        PORTC = 0X12;
        PORTD = 0X0C;
        red();
        PORTC = 0X12;
        PORTD = 0X0A;
        yellow();
        PORTC = 0X12;
        PORTD = 0X21;
        red();
        PORTC = 0X12;
        PORTD = 0X11;
        yellow();
        PORTC = 0X14;
        PORTD = 0X0C;
        red();
        PORTC = 0X12;
        PORTD = 0X09;
        yellow();
        
    }
}
