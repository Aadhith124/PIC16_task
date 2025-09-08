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
int dutycycle;
void PWM_INT();
void PWM_INC();
void PWM_DEC();

int main(){
    TRISC = 0 ;
    PWM_INT();
    while(1){
       
    }
}
void PWM_INT(){
    PR2 = 160;
    CCP1CON = 0X2C;
    CCPR1L  = 80;
    T2CON = 0X06;
    
}





