#include "stage2led.h"

// stage: 20 * 24

// tetris               => red
// bomb                 => blue
// the place clean bomb => purple(red+blue)

void LEDinit(LED *led) {
    int i;
    for(i=0; i<72; ++i) {
        led->blue[i/3][i%3] = 0x00;
        led->red[i/3][i%3] = 0x00;
    }
    return;
}

void stage2led(uint8_t stage1[][10], uint8_t stage2[][10], LED *led) {
    LEDinit(led);
    
    int i, j;
    /*
     * setting stage
     *
     * stage 1, col 0 to 6 are at first metrix col
     * col 7 to 9 are at second metrix col
     *
     * stage 2, col 0 to 2 are at second metrix
     * col 3 to 9 are at third metrix
    */

    // stage 1
    for(i=0; i<20; ++i) {
        for(j=0; j<10; ++j) {
            if(j < 7) {
                // first metrix
                if(stage1[i][j] == 1) {
                    // tetris => red
                    led->red[i+4][0] |= (1 << (6-j));
                }else if(stage1[i][j] == 2) {
                    // bomb => blue
                    led->blue[i+4][0] |= (1 << (6-j));
                }else if(stage1[i][j] == 3) {
                    // the place clean bomb => red + blue
                    led->red[i+4][0] |= (1 << (6-j));
                    led->blue[i+4][0] |= (1 << (6-j));
                }
            }else{
                // second metrix
                if(stage1[i][j] == 1) {
                    // tetris => red
                    led->red[i+4][1] |= (1 << (14-j));
                }else if(stage1[i][j] == 2) {
                    // bomb => blue
                    led->blue[i+4][1] |= (1 << (14-j));
                }else if(stage1[i][j] == 3) {
                    // the place clean bomb => red + blue
                    led->red[i+4][1] |= (1 << (14-j));
                    led->blue[i+4][1] |= (1 << (14-j));
                }
            }
        }
    }

    // stage 2
    for(i=0; i<20; ++i) {
        for(j=0; j<10; ++j) {
            if(j < 3) {
                // second metrix
                if(stage2[i][j] == 1) {
                    // tetris => red
                    led->red[i+4][1] |= (1 << (2-j));
                }else if(stage2[i][j] == 2) {
                    // bomb => blue
                    led->blue[i+4][1] |= (1 << (2-j));
                }else if(stage2[i][j] == 3) {
                    // the place clean bomb => red + blue
                    led->red[i+4][1] |= (1 << (2-j));
                    led->blue[i+4][1] |= (1 << (2-j));
                }
            }else{
                // third metrix
                if(stage2[i][j] == 1) {
                    // tetris => red
                    led->red[i+4][2] |= (1 << (10-j));
                }else if(stage2[i][j] == 2) {
                    // bomb => blue
                    led->blue[i+4][2] |= (1 << (10-j));
                }else if(stage2[i][j] == 3) {
                    // the place clean bomb => red + blue
                    led->red[i+4][2] |= (1 << (10-j));
                    led->blue[i+4][2] |= (1 << (10-j));
                }
            }
        }
    }

    return;
}

void finalLed(PLAYER *p1, PLAYER *p2, LED *led) {
    LEDinit(led);
    // check who win
    int status = 0; // status 0 => draw, 1 => p1 win, 2 => p2 win
    if(p1->ko > p2->ko) status = 1;
    else if(p1->ko < p2->ko) status = 2;
    else {
        if(p1->point > p2->point)  status = 1;
        else if(p1->point < p2->point) status = 2;
    }
    uint8_t allLight[20][10], noLight[20][10];
    for(int i=0; i<200; ++i) {
      allLight[i/10][i%10] = 3;  
      noLight[i/10][i%10] = 1;  
    }
    switch(status) {
        case 1:
            stage2led(allLight, noLight, led);
            break;
        case 2:
            stage2led(noLight, allLight, led);
            break;
        case 0:
            stage2led(allLight, allLight, led);
            break;
        default: break;
    }

    return;
}
