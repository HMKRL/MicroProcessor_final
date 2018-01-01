#ifndef PLAYER_H
#define PLAYER_H

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



#endif /* ifndef PLAYER_H */
