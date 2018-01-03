#include "player.h"

void PLAYERinit(PLAYER *p) {
    p->point = 0;
    p->usedPoint = 0;
    p->bomb = 0;
    p->ko = 0;
    p->combo = 0;

    p->tetris_cur.type = 'N';
    p->tetris_pre.type = 'N';
    STAGEinit(p->stage);

}

