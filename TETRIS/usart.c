#include "usart.h"

#include <xc.h>
#include <pic18f4520.h>

void USART_init(void) {

    /* setting portC pin6/7 */
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;

    /* setting baud rate 9600 */
    BAUDCONbits.BRG16 = 1;  // read baud rate table
    TXSTAbits.BRGH = 1;
    SPBRG = 25;
    TRISD = 0;

    TXSTAbits.SYNC = 0;     // async mode
    RCSTAbits.SPEN = 1;     // open serial port

    /* setting TX/RX */
    PIR1bits.TXIF = 1;
    PIR1bits.RCIF = 0;
    TXSTAbits.TXEN = 1;     // enable TX
    RCSTAbits.CREN = 1;     // enable RX

    /* setting TX/RX interrupt */
    PIE1bits.TXIE = 0;
    IPR1bits.TXIP = 0;      // low priority interrupt
    PIE1bits.RCIE = 1;
    IPR1bits.RCIP = 1;      // high priority interrupt
}

void USART_ISRinit(void) {
    RCONbits.IPEN = 1;      // enable interrupt priorty
    INTCONbits.GIEH = 1;    // enable high priority interrupt
    INTCONbits.GIEL = 1;    // enable low priority interrupt
}

char USART_read(void) {
    return RCREG;
}
