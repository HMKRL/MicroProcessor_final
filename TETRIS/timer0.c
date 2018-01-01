#include "timer0.h"

#include <xc.h>
#include <pic18f4520.h>

void settimer0(int ms) {
    int i;
    // prescaler => 1:8
    T0CON = 0x82;
    TMR0 = ms;
    INTCONbits.TMR0IF = 0;
    return;
}

void resetTimer0(int ms) {
    TMR0 = ms;
    INTCONbits.TMR0IF = 0;
    return;
}
