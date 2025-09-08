
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

int Tx_index = 0;
int Tx_start = 0;
const char arr[10] = "hello ";

void __interrupt() isr(void){
    if(TXIE && TXIF){
        if(arr[Tx_index]!='\0'){
            TXREG  = arr[Tx_index];
            Tx_index++;
        }
        TXIF = 0;
    }
}

void start_trans(){
    if(!Tx_start){
        Tx_start = 1;
        Tx_index  = 0;
        TXREG  = arr[Tx_index++];
        TXIE  = 1;
    }
}
void main(){
    
    TRISC6 = 0;
    TRISC7 = 1;
    INTCON  = 0XC0;
    PIE1 = 0X00;
    TXSTA = 0X24;
    RCSTA = 0X80;
    SPBRG = 129;
    TXIE = 0;
    while(1){
        start_trans();
        while(Tx_start){
           __delay_ms(10); 
        }
        
        __delay_ms(500);
     }
}