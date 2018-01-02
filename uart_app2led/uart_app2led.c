/* 
 * File:   uart_app2led.c
 * Author: hty
 *
 * Created on January 1, 2018, 10:50 PM
 */

#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

#include <xc.h>
#include <pic18f4520.h>

#include <stdlib.h>
#include <string.h>

#include "app2led.h"

#define _XTAL_FREQ 8000000
#define DELAY() __delay_ms(500)

#define mRow 1
#define mCol 3
#define aRow 8
#define aCol 3
#define NUM_DATA 24

static char red [aRow][aCol];
static char blue[aRow][aCol];
static int  red_ready;
static int  blue_ready;
static enum {IDLE, RED, BLUE, WAIT} state;

static void init();

int main() {
    init();
    
    while(1);

    return (EXIT_SUCCESS);
}

static void init()
{
    /* set OSC to 8MHz */
    OSCCONbits.IRCF = 7;
    /* set baud rate to 115200 */
    TXSTAbits.SYNC = 0;
    BAUDCONbits.BRG16 = 1;
    TXSTAbits.BRGH = 1;
    SPBRGH = 0;
    SPBRG = 16;
    /* enable serial port */
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
    TRISC7 = 1;
    TRISC6 = 1;
    /* enable serial port interrupt */
    GIE  = 1;
    PEIE = 0;
    TXIP = 1;
    TXIE = 1;
    RCIP = 1;
    RCIE = 1;
    /* other init */
    app2led_init(mRow, mCol);
    memset(red , 0, 24);
    memset(blue, 0, 24);
    red_ready  = 0;
    blue_ready = 0;
    state = IDLE;
    /* set detect led */
    TRISD7 = 0;
    TRISD6 = 0;
    TRISD5 = 0;
    /* enable interrupt */
    IPEN = 1;
}

static void write_uart(char data)
{
    TXREG = data;
}

static char read_uart()
{
    return RCREG;
}

void interrupt HI_ISR()
{
    if (RCIE && RCIF) {
        switch (state) {
            case IDLE:
                if (read_uart() == 'S') {
                    write_uart('S');
                    
                    state = RED;
                }
                break;
            case RED:
                if (red_ready != NUM_DATA) {
                    char *red_ptr = &red[0][0] + red_ready++;
                    *red_ptr = read_uart();
                    if (red_ready == NUM_DATA) {
                       red_ready = 0;
                        state = BLUE;
                    }
                }
                break;
            case BLUE:
                if (blue_ready != NUM_DATA) {
                    char *blue_ptr = &blue[0][0] + blue_ready++;
                    *blue_ptr = read_uart();
                    if (blue_ready == NUM_DATA) {
                        blue_ready = 0;
                        write_uart('E');
                        state = WAIT;
                    }
                }
                break;
            case WAIT:
                if (read_uart() == 'E') {
                    app2led_setLed(&red[0][0], &blue[0][0]);
                    state = IDLE;
                }
                break;
        }
    }
    
    switch (state) {
            case IDLE:
                LATD7 = 0;
                LATD6 = 0;
                LATD5 = 0;
                break;
            case RED:
                LATD7 = 0;
                LATD6 = 0;
                LATD5 = 1;
                break;
            case BLUE:
                LATD7 = 0;
                LATD6 = 1;
                LATD5 = 0;
                break;
            case WAIT:
                LATD7 = 0;
                LATD6 = 1;
                LATD5 = 1;
                break;
        }
}