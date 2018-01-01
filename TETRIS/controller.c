#include "controller.h"

#include <stdio.h>

bool CONTROLLER_newTetris(PLAYER *p) {
    POSITION pos;
    pos.row = 0;
    pos.col = 4;
    // for first round
    int index;
    if(p->firstround) {
        while(FirstRound[index = rand() % 7]);
        FirstRound[index] = true;
        p->firstround--;
    }
    // random
    else{
        index = rand() % 7;
    }
    TETRISinit(&(p->tetris_cur), TetrisType[index], pos);
    p->tetris_pre.type = 'N';

    if(STAGE_TETRIScheck(p->stage, &(p->tetris_pre), &(p->tetris_cur))) {
        STAGE_TETRISupdate(&(p->tetris_pre), &(p->tetris_cur), p->stage);
        memcpy(&(p->tetris_pre), &(p->tetris_cur), sizeof(TETRISMODULE));
        return true;
    }
    return false;
}

bool CONTROLLER_autodrop(PLAYER *p) {
    TETRIS_move(&(p->tetris_cur), 'D');
    if(STAGE_TETRIScheck(p->stage, &(p->tetris_pre), &(p->tetris_cur))) {
        STAGE_TETRISupdate(&(p->tetris_pre), &(p->tetris_cur), p->stage);
        memcpy(&(p->tetris_pre), &(p->tetris_cur), sizeof(TETRISMODULE));
        return true;
    }
    return false;
}

void CONTROLLER_method(PLAYER *p, char type) {
    /* method: r, F, D, L, R
     *
     * r: rotate
     * F: drop to bottom
     * D: move down
     * L: move left
     * R: move right
     *
     */
    if(type == 'r') {
        TETRIS_rotate(&(p->tetris_cur));
    }else if(type == 'F'){
        int rowDiff = STAGE_findDropDiff(&(p->tetris_cur), p->stage);
        TETRIS_Addrow(&(p->tetris_cur), rowDiff);
    }else{
        TETRIS_move(&(p->tetris_cur), type);
    }
    if(STAGE_TETRIScheck(p->stage, &(p->tetris_pre), &(p->tetris_cur))) {
        STAGE_TETRISupdate(&(p->tetris_pre), &(p->tetris_cur), p->stage);
        memcpy(&(p->tetris_pre), &(p->tetris_cur), sizeof(TETRISMODULE));
    }else{
        memcpy(&(p->tetris_cur), &(p->tetris_pre), sizeof(TETRISMODULE));
    }
    return;
}

void CONTROLLER_stageUpdate(PLAYER *p) {
    int line = STAGE_STAGEcheck(p->stage);
    if(!line) {
        p->combo = 0;
    }else{
        p->point += POINT_getpoint(line, p->combo);
        p->combo++;
    }
}

void CONTROLLER_checkbomb(PLAYER *p) {
    int line = STAGE_cleanbomb(p->stage, &(p->tetris_cur), p->bomb);
    // get point
    p->point += POINT_getpoint(line, p->combo);
    // update bomb
    p->bomb -= line;
    if(line) p->combo++;
}

void CONTROLLER_givebomb(PLAYER *send, PLAYER *recv) {
    int bombRowNum = (send->point - send->usedPoint) / 50;
    send->usedPoint += bombRowNum * 50;
    int bombnum = STAGE_setbomb(recv->stage, recv->bomb, bombRowNum);
    if(bombnum == 0) {
        send->ko += 1;
    }
    recv->bomb = bombnum;
}
