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
unsigned char sec,min,hour,day,date,month,year;
void rtc_int();
void rtc_start();
void rtc_stop();
void rtc_ack();
void rtc_nak();
void rtc_res();
void rtc_send(unsigned char a);
void rtc_send_byte(unsigned char addr,unsigned char data);
unsigned char rtc_read();
unsigned char rtc_read_byte(unsigned char addr);
void waitmssp();
unsigned char convup(unsigned char bcd);
unsigned char convd(unsigned char bcd);
#define rs RB0
#define rw RB1
#define en RB2
#define delay for(i=0;i<1000;i++)
int i;
void lcd_init();
void cmd(unsigned char a);
void dat(unsigned char b);
void show(unsigned char *s);
void lcd_init()
{
    TRISD=TRISB=0;
    cmd(0x38);
    cmd(0x0c);
    cmd(0x06);
    cmd(0x80);
}
void cmd(unsigned char a)
{
    PORTD=a;
    rs=0;
    rw=0;
    en=1;
    delay;
    en=0;
}
void dat(unsigned char b)
{
    PORTD=b;
    rs=1;
    rw=0;
    en=1;
    delay;
    en=0;
}
void show(unsigned char *s)
{
    while(*s) {
        dat(*s++);
    }
}






void main()
{
    lcd_init();
    
    show("Time:");
    
    rtc_int();
    while(1) {
        sec  =rtc_read_byte(0x00);
        min  =rtc_read_byte(0x01);
        hour =rtc_read_byte(0x02);
        
        
        cmd(0x85);
        dat(convup(hour));
        dat(convd(hour));
        dat(':');
        dat(convup(min));
        dat(convd(min));
        dat(':');
        dat(convup(sec));
        dat(convd(sec));
        
        
            
    }
}       
    

void rtc_int()
{
    TRISC3=TRISC4=1;
    SSPCON=0x28;
    SSPADD= (((11059200/4)/100)-1);
}

void waitmssp()
{
    while(!SSPIF); // SSPIF is zero while TXion is progress    
    SSPIF=0;
}

void rtc_send(unsigned char a)
{
    SSPBUF=a;
    waitmssp();
    while(ACKSTAT);
}   

void rtc_send_byte(unsigned char addr,unsigned char data)
{
    rtc_start();
    rtc_send(0xD0);
    //rtc_send(addr>>8);
    rtc_send(addr);
    rtc_send(data);
    rtc_stop();
}

unsigned char rtc_read()
{
    RCEN=1;
    waitmssp();
    return SSPBUF;
}   
    
unsigned char rtc_read_byte(unsigned char addr)
{
    unsigned char rec;
L:  rtc_res();
    SSPBUF=0xD0;
    waitmssp();
    while(ACKSTAT){goto L;}
    //rtc_send(addr>>8);
    rtc_send(addr);
    rtc_res();
    rtc_send(0xD1);
    rec=rtc_read();
    rtc_nak();
    rtc_stop();
    return rec;
}
    
    
void rtc_start()
{
    SEN=1;
    waitmssp();
}

void rtc_stop()
{
    PEN=1;
    waitmssp();
}

void rtc_res()
{
    RSEN=1;
    waitmssp();
}

void rtc_ack()
{
    ACKDT=0;
    ACKEN=1;
    waitmssp();
}

void rtc_nak()
{
    ACKDT=1;
    ACKEN=1;
    waitmssp();
}

unsigned char convup(unsigned char bcd)
{ 
    return ((bcd>>4)+48);
}

unsigned char convd(unsigned char bcd)
{ 
    return ((bcd&0x0F)+48);
}