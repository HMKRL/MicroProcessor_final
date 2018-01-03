#include "stage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int stage
// 0 => no led
// 1 => tetris
// 2 => bomb
// 3 => the place to clear bomb

void STAGEinit(uint8_t stage[][10]) {
    int i;
    for(i=0; i<20; ++i) {
        memset(stage[i], 0, sizeof(stage[i]));
    }
    return;
}

bool STAGE_TETRIScheck(uint8_t stage[][10], TETRISMODULE *pre, TETRISMODULE *cur) {
    int i, temp_stage[20][10];
    for(i=0; i<200; ++i) temp_stage[i/10][i%10] = stage[i/10][i%10];
    for(i=0; i<4; ++i) temp_stage[pre->pos[i].row][pre->pos[i].col] = 0;
    for(i=0; i<4; ++i) {
        // check whether out of range or not
        if(cur->pos[i].row < 0 || cur->pos[i].row > 19 || cur->pos[i].col < 0 || cur->pos[i].col > 9) return false;
        // check whether the space is used or not
        if(temp_stage[cur->pos[i].row][cur->pos[i].col]) {
            return false;
        }
    }

    return true;
}

int STAGE_tetrisROWcheck(uint8_t stage[][10], int index) {
    int i, check=0;
    for(i=0; i<10; ++i) {
        if(stage[index][i]) check++;
    }
    return check;
}

int STAGE_findDropDiff(TETRISMODULE *t, uint8_t stage[][10]) {
    int i, j, diff=20;
    // detect four times
    for(i=0; i<4; ++i) {
        for(j=1; j<20; ++j) {
            if(stage[t->pos[i].row+j][t->pos[i].col]) break;
        }
        // update diff if is not 0, find minimum
        if(j-1 != 0) {
            diff = (j-1<diff)?j-1:diff;
        }
    }
    // diff == 20 => not update => so diff = 0
    if(diff == 20)  return 0;
    else return diff;
}

int STAGE_STAGEcheck(uint8_t stage[][10]) {
    int i, j, line=0;
    for(i=0; i<20; ++i) {
        // check row is full tetris or not
        if(STAGE_tetrisROWcheck(stage, i) == 10) {
            // clear row
            for(j=0; j<10; ++j) stage[i][j] = 0;
            // tetris auto drop
            for(j=i; j>0; --j) {
                memcpy((int*)stage[j], (int*)stage[j-1], sizeof(stage[j]));
            }
            // clean row 0
            for(j=0; j<10; ++j) stage[0][j] = 0;
            line++;
        }
    }
    return line;
}

void STAGE_TETRISupdate(TETRISMODULE *pre, TETRISMODULE *cur, uint8_t stage[][10]) {
    int i;
    // clean pre
    if(pre->type != 'N') {
        for(i=0; i<4; ++i) {
            stage[pre->pos[i].row][pre->pos[i].col] = 0;
        }
    }
    // set cur
    for(i=0; i<4; ++i) {
        stage[cur->pos[i].row][cur->pos[i].col] = 1;
    }
    return;
}

int STAGE_setbomb(uint8_t stage[][10], int curBomb, int addBomb) {
    // check the space for addBomb
    int i;
    for(i=0; i<addBomb; ++i) {
        // have no space
        if(STAGE_tetrisROWcheck(stage, i)) {
            STAGE_cleanAllbomb(stage, curBomb);
            return 0;
        }
    }
    // copy data
    for(i=0; i<=19-addBomb; ++i) {
        memcpy(stage[i], stage[i+addBomb], sizeof(stage[i]));
    }
    // set bomb
    int j, randomBomb;
    for(i=19; i>19-addBomb; --i) {
        // random bomb place that can clean
        randomBomb = rand() % 10;
        for(j=0; j<10; ++j) stage[i][j] = 2;
        stage[i][randomBomb] = 3;
    }
    return curBomb+addBomb;
}

void STAGE_cleanAllbomb(uint8_t stage[][10], int bomb) {
    // clean all bomb
    int i;
    for(i=19; i>19-bomb; --i) {
        memset(stage[i], 0, sizeof(stage[i]));
    }
    // copy data
    for(i=19; i>bomb; --i) {
        memcpy(stage[i], stage[i-bomb], sizeof(stage[i]));
    }
    // clean
    for(i=0; i<bomb; ++i)
    memset(stage[i], 0, sizeof(stage[i]));
}

int STAGE_cleanbomb(uint8_t stage[][10], TETRISMODULE *t, int curBomb) {
    int i, j, k, times=0;
    bool flag;
    // detect integer curBomb times
    // if cannot clear bomb, break and return
    for(i=0; i<curBomb; ++i) {
        flag = false;
        for(j=0; j<4; ++j) {
            // detect the thing below the tetris
            if(stage[t->pos[j].row+1][t->pos[j].col] == 3) {
                flag = true;
                times++;
                // clean bomb
                memset(stage[t->pos[j].row+1], 0, sizeof(stage[t->pos[j].row+1]));
                // copy data(tetris auto drop)
                for(k=t->pos[j].row+1; k>0; k--) {
                    memcpy(stage[k], stage[k-1], sizeof(stage[k]));
                }
                memset(stage[0], 0, sizeof(stage[0]));
                break;
            }
        }
        if(!flag) break;
    }
    return times;
}
