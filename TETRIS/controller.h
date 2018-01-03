#ifndef CONTROLLER_H
#define CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TetrisModule.h"
#include "stage.h"
#include "point.h"
#include "player.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

    static const char TetrisType[] = {'I', 'T', 'O', 'S', 'J', 'Z', 'L'};
    static bool FirstRound[7] = {false};

    bool CONTROLLER_newTetris(PLAYER*);
    bool CONTROLLER_autodrop(PLAYER*);
    void CONTROLLER_method(PLAYER*, char);
    void CONTROLLER_stageUpdate(PLAYER*);
    void CONTROLLER_checkbomb(PLAYER*);
    void CONTROLLER_givebomb(PLAYER*, PLAYER*);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* ifndef CONTROLLER_H */
