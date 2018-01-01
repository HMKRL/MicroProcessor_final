#ifndef TETRISMODULE_H
#define TETRISMODULE_H

#include <stdlib.h>

typedef struct position{
    int row;
    int col;
}POSITION;

typedef struct _tetrismodule{
    char type;
    POSITION pos[4], rotateC;
}TETRISMODULE;

void TETRISinit(TETRISMODULE*, char, POSITION);
void TETRIS_move(TETRISMODULE*, char);
void TETRIS_rotate(TETRISMODULE*);
void TETRIS_Addrow(TETRISMODULE*, int);


#endif /* ifndef TETRISMODULE_H */
