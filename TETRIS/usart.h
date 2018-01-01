#ifndef USART_H
#define USART_H

#include <string.h>
#include <xc.h>

#include <pic18f4520.h>

#pragma config OSC = INTIO67
#pragma config FCMEN = OFF
#pragma config IESO = ON

void USART_init(void);
char USART_read(void);
void USART_ISRinit(void);

#endif /* ifndef USART_H */
