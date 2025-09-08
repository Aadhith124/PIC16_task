#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#include <xc.h>
#define _XTAL_FREQ 20000000
#define rs RB1
#define rw RB2
#define e RB3

void lcd();
void dat(unsigned char a);
void cmd(unsigned char a);
void display_number(unsigned int num);

void lcd(){
    cmd(0X38);
    cmd(0X0e);
    cmd(0X01);
    cmd(0X0c);
    cmd(0X80);
}
void cmd(unsigned char a){
 PORTD =a;
rs=0;
rw=0;
e=1;
__delay_ms(100);
e=0;
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
void main(){
    unsigned int value=0,result=0;
    TRISA0 = 1;
    TRISD = 0x00;
    ADCON0 = 0X01;
    ADCON1 = 0XC0;
    TRISB = 0X00;
    lcd();
    while(1){
        __delay_ms(20);
        GO_DONE = 1;
        while(GO_DONE);
        value = ADRESH;
        result  = ADRESL |value<<8;
        display_number(result);
        __delay_ms(100);
        //cmd (0X01);
        
        
    }
}