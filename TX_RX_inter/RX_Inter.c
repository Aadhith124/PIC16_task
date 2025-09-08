
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
 char a[10];
 int rx_index = 0;
 int rx_ready = 0;

#define rs RB1
#define rw RB2
#define e RB3

void lcd();
void dat(unsigned char a);
void cmd(unsigned char a);
void display_number(unsigned int num);

void lcd() {
    TRISB = 0X00;
    TRISD = 0X00;
    cmd(0X38);
    cmd(0X0e);
    cmd(0X01);
    cmd(0X0c);
    cmd(0X80);
}

void cmd(unsigned char a) {
    PORTD = a;
    rs = 0;
    rw = 0;
    e = 1;
    __delay_ms(100);
    e = 0;
}

void dat(unsigned char a) {
    PORTD = a;
    rs = 1;
    rw = 0;
    e = 1;
    __delay_ms(100); // Reduced delay
    e = 0;
}

void __interrupt() isr(){
    if(RCIF){
        if(rx_index < 10){
            a[rx_index++] = RCREG;
            if(a[rx_index-1]=='\0'||rx_index == 10){
                rx_ready = 1;
            }
        }
        else{
            RCREG;
        }
        RCIF = 0;
    }
    
}

void main() {
    
    TRISC6 = 0;
    TRISC7 = 1;
   
    lcd();
    TXSTA = 0X24;
    RCSTA = 0X90;
    INTCON  = 0XC0;
    PIE1 = 0X00;
    SPBRG = 129;
    
    int i = 0;
    while (1) {
        if(rx_ready){
            for(i=0;i< rx_index && a[i]!='\0';i++){
                dat(a[i]);
            }
            rx_index = 0;
            rx_ready = 0;
        }
        __delay_ms(10);
    }
}