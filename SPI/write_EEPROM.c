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
#define CS RC0
#define rs RB0
#define rw RC1
#define e  RC2
#define r1 RB1
#define r2 RB2
#define r3 RB3
#define c1 RB4
#define c2 RB5
#define c3 RB6
#define c5 RB7


void lcd();
void dat(unsigned char a);
void cmd(unsigned char a);
void display_number(unsigned int num);

void lcd() {
    TRISB0 = 0;
    TRISC1 = 0;
    TRISC2 = 0;
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

void display_number(unsigned int num) {
    if (num == 0) {
        dat('0');
        return;
    }
    char buffer[5];
    int i = 0;
    while (num > 0 && i < 5) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    while (i > 0) {
        dat(buffer[--i]);
    }
}
int scan_keypad() {
    TRISB = 0X70;
    r1 = 1; r2 = r3 = c1=c2=c3=c5= 0;
    int a;
    if (c1 == 1) { while (c1 == 1) return 1; }
    if (c2 == 1) { while (c2 == 1) return 4; }
    if (c3 == 1) { while (c3 == 1) return 7; }
    if (c5 == 1) { while (c5 == 1); cmd(0X01); }

    r2 = 1; r1 = r3 =c1=c2=c3=c5= 0;
    if (c1 == 1) { while (c1 == 1) return 2; }
    if (c2 == 1) { while (c2 == 1) return 5; }
    if (c3 == 1) { while (c3 == 1) return 8; }
    if (c5 == 1) { while (c5 == 1) return 0; }

    r3 = 1; r1 = r2 = c1=c2=c3=c5= 0;
    if (c1 == 1) { while (c1 == 1) return 3; }
    if (c2 == 1) { while (c2 == 1) return 6; }
    if (c3 == 1) { while (c3 == 1) cmd(0X01); }
    
    return -1;
}



void main(){
    TRISC0 = 0;
    TRISC4 = 1;
    TRISC3 = 0;
    TRISC5 = 0;
    lcd();
    SSPSTAT = 0X00;
    SSPCON = 0X20;
    while(1){
        int num;
    num = scan_keypad();
        while(num >= 0){
        CS  = 1;
    __delay_ms(100);
    CS  = 0;
     SSPBUF = 0X06;
    while(SSPIF == 0);
    SSPIF = 0;
    CS = 1;
    __delay_ms(100);
    CS = 0;
    
    
    
    SSPBUF = 0X02;
    while(SSPIF == 0);
    SSPIF = 0;
    SSPBUF = 0X00;
    while(SSPIF == 0);
    SSPIF = 0;
    SSPBUF = 0X00;
    while(SSPIF == 0);
    SSPIF = 0;
    
    SSPBUF = num;
    while(SSPIF == 0);
    SSPIF = 0;
    CS = 1;
    __delay_ms(10);
    CS=0;
    
    
    SSPBUF  = 0X03;
    while(SSPIF == 0);
    SSPIF = 0;   
    SSPBUF  = 0X00;
    while(SSPIF == 0);
    SSPIF = 0;
    SSPBUF  = 0X00;
    while(SSPIF == 0);
    SSPIF = 0;
   
    
    SSPBUF  = 0X00;
    while(SSPIF == 0);
    SSPIF = 0;
    display_number(SSPBUF);
    num = -1;
    }
   
    }
    
}