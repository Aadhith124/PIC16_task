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
#define rs RB0
#define rw RB1
#define e  RB2
char data;

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

void i2c(){
    TRISC = 0X18;
    SSPCON=0x28;
	SSPADD=0X3F;
    
}
void address(unsigned char value){
    SSPBUF=value;
	while(SSPIF==0);
	SSPIF=0;
}
void main()
{  
    char buffer[10];
    int n=10;
    i2c();
    lcd();
 	
	SEN=1;
	while(SSPIF==0);
	SSPIF=0;
    
    address(0XA0);		
	address(0X00);
	address(0X00);
  
	address('h');
    
    address('e');
    address('l');
    address('l');
    address('o');
 
	PEN=1;
	while(SSPIF==0);
	SSPIF=0;
	__delay_ms(10);
    
    SEN =1;
    while(SSPIF == 0);
    SSPIF=0;
    
    address(0XA0);	
	address(0X00);
	address(0X00);
    
    RSEN =1;
    while(SSPIF == 0);
    SSPIF=0;
    
    
    address(0XA1);
    
    int i;
    for(i=0;i<n;i++){ 
    RCEN =1;
    while(SSPIF==0);
    buffer[i] = SSPBUF;
    SSPIF = 0;
    ACKDT = (i == n - 1) ? 1 : 0; 
    ACKEN = 1;
    while(SSPIF==0);
    SSPIF = 0;
    }
    PEN = 1;
    while(SSPIF==0);  
    SSPIF = 0;
    
    
    for(i=0;i<n;i++){
        if(buffer[i]==0XFF) break;
    dat(buffer[i]);}
    
   
    __delay_ms(100);
    
    while(1);
	
}