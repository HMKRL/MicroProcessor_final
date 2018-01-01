#include "timer2.h"

#include <xc.h>
#include <pic18f4520.h>

void settimer2(int ms) {
    // prescaler => 1:4
    T2CONbits.T2OUTPS = 15;
    T2CONbits.TMR2ON = 1;
    // postscaler => 1:16
    T2CONbits.T2CKPS = 1;
    PR2 = ms;
    PIR1bits.TMR2IF = 0;
}

void resetTimer2(int ms) {
    PR2 = ms;
    PIR1bits.TMR2IF = 0;
}


