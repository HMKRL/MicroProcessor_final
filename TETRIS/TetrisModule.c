#include "TetrisModule.h"

void TETRISinit(TETRISMODULE *t, char type, POSITION pos){
    /* begin init */
    /* type, pos, rotateC */
    t->type = type;
    switch (t->type) {
        case 'I':
            /* type:
             *
             * xxxx
             *
             *
             */
            /* set position */
            t->pos[0].row = pos.row;
            t->pos[0].col = pos.col;
            t->pos[1].row = pos.row;
            t->pos[1].col = pos.col+1;
            t->pos[2].row = pos.row;
            t->pos[2].col = pos.col+2;
            t->pos[3].row = pos.row;
            t->pos[3].col = pos.col+3;
            /* set rotate center */
            t->rotateC = t->pos[1];
            break;
        case 'J':
            /* type:
             *
             * xxx
             *   x
             *
             */
            /* set position */
            t->pos[0].row = pos.row;
            t->pos[0].col = pos.col;
            t->pos[1].row = pos.row;
            t->pos[1].col = pos.col+1;
            t->pos[2].row = pos.row;
            t->pos[2].col = pos.col+2;
            t->pos[3].row = pos.row+1;
            t->pos[3].col = pos.col+2;
            /* set rotate center */
            t->rotateC = t->pos[2];
            break;
        case 'L':
            /* type:
             *
             * xxx
             * x
             *
             */
            /* set position */
            t->pos[0].row = pos.row;
            t->pos[0].col = pos.col;
            t->pos[1].row = pos.row;
            t->pos[1].col = pos.col+1;
            t->pos[2].row = pos.row;
            t->pos[2].col = pos.col+2;
            t->pos[3].row = pos.row+1;
            t->pos[3].col = pos.col;
            /* set rotate center */
            t->rotateC = t->pos[0];
            break;
        case 'O':
            /* type:
             *
             * xx
             * xx
             *
             */
            /* set position */
            t->pos[0].row = pos.row;
            t->pos[0].col = pos.col;
            t->pos[1].row = pos.row;
            t->pos[1].col = pos.col+1;
            t->pos[2].row = pos.row+1;
            t->pos[2].col = pos.col;
            t->pos[3].row = pos.row+1;
            t->pos[3].col = pos.col+1;
            /* set rotate center */
            /* o type no need rotate => set {-1,-1} */
            t->rotateC.row = -1;
            t->rotateC.col = -1;
            break;
        case 'S':
            /* type:
             *
             *  xx
             * xx
             *
             */
            /* set position */
            t->pos[0].row = pos.row;
            t->pos[0].col = pos.col;
            t->pos[1].row = pos.row;
            t->pos[1].col = pos.col+1;
            t->pos[2].row = pos.row+1;
            t->pos[2].col = pos.col-1;
            t->pos[3].row = pos.row+1;
            t->pos[3].col = pos.col;
            /* set rotate center */
            t->rotateC = t->pos[3];
            break;
        case 'T':
            /* type:
             *
             * xxx
             *  x
             *
             */
            /* set position */
            t->pos[0].row = pos.row;
            t->pos[0].col = pos.col;
            t->pos[1].row = pos.row;
            t->pos[1].col = pos.col+1;
            t->pos[2].row = pos.row;
            t->pos[2].col = pos.col+2;
            t->pos[3].row = pos.row+1;
            t->pos[3].col = pos.col+1;
            /* set rotate center */
            t->rotateC = t->pos[1];
            break;
        case 'Z':
            /* type:
             *
             * xx
             *  xx
             *
             */
            /* set position */
            t->pos[0].row = pos.row;
            t->pos[0].col = pos.col;
            t->pos[1].row = pos.row;
            t->pos[1].col = pos.col+1;
            t->pos[2].row = pos.row+1;
            t->pos[2].col = pos.col+1;
            t->pos[3].row = pos.row+1;
            t->pos[3].col = pos.col+2;
            /* set rotate center */
            t->rotateC = t->pos[2];
            break;
        default: break;
    }
    return;
}

void TETRIS_move(TETRISMODULE *t, char movetype) {
    int i;
    // move method
    // L => move left
    // R => move right
    // D => move down
    switch (movetype) {
        case 'L':
            for(i=0; i<4; ++i) {
                t->pos[i].col -= 1;
            }
            t->rotateC.col -= 1;
            break;
        case 'R':
            for(i=0; i<4; ++i) {
                t->pos[i].col += 1;
            }
            t->rotateC.col += 1;
            break;
        case 'D':
            for(i=0; i<4; ++i) {
                t->pos[i].row += 1;
            }
            t->rotateC.row += 1;
            break;
        default: break;
    }
    return;
}

void TETRIS_rotate(TETRISMODULE *t) {
    // rotate +90 degree
    int i;
    if(t->type == 'O') {
        // type o
        return;
    }
    int rowDiff, colDiff;
    for (i = 0; i < 4; ++i) {
        rowDiff = t->pos[i].row - t->rotateC.row;
        colDiff = t->pos[i].col - t->rotateC.col;
        // divide into four Quadrant
        // center => needn't rotate
        if(rowDiff == 0 && colDiff == 0) continue;
        // point at x line => to y line
        else if(rowDiff == 0 && colDiff != 0) {
            t->pos[i].row = t->rotateC.row + colDiff;
            t->pos[i].col = t->rotateC.col;
        }
        // point at y line => to x line
        else if(rowDiff != 0 && colDiff == 0) {
            t->pos[i].row = t->rotateC.row;
            t->pos[i].col = t->rotateC.col - rowDiff;
        }
        // point at first quadrant => to forth quadrant
        else if(rowDiff < 0 && colDiff > 0) {
            t->pos[i].row = t->rotateC.row - rowDiff;
        }
        // point at second quadrant => to first quadrant
        else if(rowDiff < 0 && colDiff < 0) {
            t->pos[i].col = t->rotateC.col - colDiff;
        }
        // point at third quadrant => to second quadrant
        else if(rowDiff > 0 && colDiff < 0) {
            t->pos[i].row = t->rotateC.row - rowDiff;
        }
        // point at forth quadrant => to third quadrant
        else{
            t->pos[i].col = t->rotateC.col - colDiff;
        }
    }
    return;
}

void TETRIS_Addrow(TETRISMODULE *t, int row) {
    int i;
    // go to row
    for(i=0; i<4; ++i) {
        t->pos[i].row += row;
    }
    return;
}
