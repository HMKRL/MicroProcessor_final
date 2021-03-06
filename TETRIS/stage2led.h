#ifndef STAGE2LED_H
#define STAGE2LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "player.h"

#include <stdbool.h>
#include <stdint.h>


    typedef struct LED{
        char blue[24][3];
        char red[24][3];
    }LED;

    void LEDinit(LED*);
    void stage2led(uint8_t[][10], uint8_t[][10], LED*);
    void finalLed(PLAYER*, PLAYER*, LED*);

#ifdef __cplusplus
} // extern "C"
#endif



#endif /* ifndef STAGE2LED_H */
