#ifndef STAGE_H
#define STAGE_H

#include "TetrisModule.h"

#include <stdbool.h>
#include <stdint.h>


void STAGEinit(uint8_t[][10]);

bool STAGE_TETRIScheck(uint8_t[][10], TETRISMODULE*, TETRISMODULE*);

int STAGE_tetrisROWcheck(uint8_t[][10], int);

// stage check(&update) and return line count of full tetris
int STAGE_STAGEcheck(uint8_t[][10]);

// find diff between cur tetris and the bottom of stage
int STAGE_findDropDiff(TETRISMODULE*, uint8_t[][10]);

// update stage
void STAGE_TETRISupdate(TETRISMODULE*, TETRISMODULE*, uint8_t[][10]);

// set bomb
int STAGE_setbomb(uint8_t[][10], int, int);

// clean all bomb
void STAGE_cleanAllbomb(uint8_t[][10], int);

// detect and clean bomb
int STAGE_cleanbomb(uint8_t[][10], TETRISMODULE *t, int);


#endif /* ifndef STAGE_H */
