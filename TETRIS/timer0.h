#ifndef TIMER0_H
#define TIMER0_H

#define _10ms  65222
#define _50ms  63972
#define _100ms 62410
#define _200ms 59285
#define _500ms 49910
#define _1s    34285

#include <pic18f4520.h>
#include <xc.h>

void settimer0(int);
void resetTimer0(int);


#endif /* ifndef TIMER0_H */
