// PIC16F877A Configuration Bit Settings
#pragma config FOSC = HS        // HS oscillator
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = OFF      // Power-up Timer disabled
#pragma config BOREN = OFF      // Brown-out Reset disabled
#pragma config LVP = OFF        // Low-Voltage Programming disabled
#pragma config CPD = OFF        // Data EEPROM code protection off
#pragma config WRT = OFF        // Flash Program Memory write protection off
#pragma config CP = OFF         // Flash Program Memory code protection off

#include <xc.h>

#define rs RC0
#define rw RC1
#define e RC2
#define _XTAL_FREQ 20000000
#define r1 RD0
#define r2 RD1
#define r3 RD2
#define r4 RD3
#define c1 RD4
#define c2 RD5
#define c3 RD6
#define c4 RD7

int nums1, nums2, result;
char operation = 0; // Tracks current operation (+, -, *, /)

// Function to send LCD command
void cmd(unsigned char a) {
    PORTB = a;
    rs = 0;
    rw = 0;
    e = 1;
    __delay_ms(2); // Reduced delay for most commands
    e = 0;
    if (a == 0x01 || a == 0x02) {
        __delay_ms(2); // Longer delay for clear and home commands
    } else {
        __delay_us(50); // Short delay for other commands
    }
}

// Function to send LCD data
void dat(unsigned char a) {
    PORTB = a;
    rs = 1;
    rw = 0;
    e = 1;
    __delay_ms(2); // Reduced delay
    e = 0;
}

// Function to initialize LCD
void lcd() {
    cmd(0x38); // 2 lines, 5x7 matrix
    cmd(0x0E); // Display on, cursor on
    cmd(0x01); // Clear display
    cmd(0x06); // Entry mode: increment cursor
    cmd(0x0C); // Display on, cursor off
    cmd(0x80); // Set cursor to first line
}

// Function to display an integer on LCD
void display_number(int num) {
    if (num == 0) {
        dat('0');
        return;
    }
    char buffer[10];
    int i = 0;
    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    for (int j = i - 1; j >= 0; j--) {
        dat(buffer[j]);
    }
}

// Function to display error message
void display_error(const char *msg) {
    cmd(0x01); // Clear display
    for (int i = 0; msg[i] != '\0'; i++) {
        dat(msg[i]);
    }
}

// Function to scan keypad and return key value or operation
int scan_keypad() {
    r1 = 1; r2 = r3 = r4 = 0;
    if (c1 == 1) { while (c1 == 1); dat('7'); return 7; }
    if (c2 == 1) { while (c2 == 1); dat('8'); return 8; }
    if (c3 == 1) { while (c3 == 1); dat('9'); return 9; }
    if (c4 == 1) { while (c4 == 1); dat('/'); return '/'; }

    r2 = 1; r1 = r3 = r4 = 0;
    if (c1 == 1) { while (c1 == 1); dat('4'); return 4; }
    if (c2 == 1) { while (c2 == 1); dat('5'); return 5; }
    if (c3 == 1) { while (c3 == 1); dat('6'); return 6; }
    if (c4 == 1) { while (c4 == 1); dat('*'); return '*'; }

    r3 = 1; r1 = r2 = r4 = 0;
    if (c1 == 1) { while (c1 == 1); dat('1'); return 1; }
    if (c2 == 1) { while (c2 == 1); dat('2'); return 2; }
    if (c3 == 1) { while (c3 == 1); dat('3'); return 3; }
    if (c4 == 1) { while (c4 == 1); dat('-'); return '-'; }

    r4 = 1; r1 = r2 = r3 = 0;
    if (c1 == 1) { while (c1 == 1); cmd(0x01); return 'C'; } // Clear
    if (c2 == 1) { while (c2 == 1); dat('0'); return 0; }
    if (c3 == 1) { while (c3 == 1); dat('='); return '='; }
    if (c4 == 1) { while (c4 == 1); dat('+'); return '+'; }

    return -1; // No key pressed
}

void main() {
    TRISB = 0x00;   // PORTB as output for LCD data
    TRISC0 = 0;     // RS as output
    TRISC1 = 0;     // RW as output
    TRISC2 = 0;     // E as output
    TRISD = 0xF0;   // RD0-RD3 outputs (rows), RD4-RD7 inputs (columns)
    nums1 = nums2 = result = 0; // Initialize variables
    operation = 0;
    lcd();

    while (1) {
        int key = scan_keypad();
        if (key >= 0 && key <= 9) {
            if (operation == 0) {
                nums1 = key; // Store first number
            } else {
                nums2 = key; // Store second number
            }
        } else if (key == '+' || key == '-' || key == '*' || key == '/') {
            operation = key; // Store operation
        } else if (key == '=') {
            if (operation == 0) {
                continue; // No operation selected
            }
            switch (operation) {
                case '+':
                    result = nums1 + nums2;
                    break;
                case '-':
                    result = nums1 - nums2;
                    break;
                case '*':
                    result = nums1 * nums2;
                    break;
                case '/':
                    if (nums2 == 0) {
                        display_error("Err: Div0");
                        operation = 0; // Reset operation
                        continue;
                    }
                    result = nums1 / nums2;
                    break;
            }
            display_number(result);
            operation = 0; // Reset operation after displaying result
        } else if (key == 'C') {
            nums1 = nums2 = result = 0;
            operation = 0; // Reset calculator state
        }
    }
}