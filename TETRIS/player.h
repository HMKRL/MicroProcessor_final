#ifndef PLAYER_H
#define PLAYER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "stage.h"
#include "TetrisModule.h"

    typedef struct _player{
        int combo;
        int point;
        TETRISMODULE tetris_cur, tetris_pre;
        uint8_t stage[20][10];
        int ko;
        int bomb;
        int usedPoint;
        // check first round or not
        int firstround;
    }PLAYER;

    void PLAYERinit(PLAYER*);

#ifdef __cplusplus
} // extern "C"
#endif




#endif /* ifndef PLAYER_H */
