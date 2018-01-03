#include "TetrisModule.h"
#include "player.h"
#include "stage.h"
#include "controller.h"
#include "point.h"
#include "stage2led.h"

#include <time.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x04

// for reset
const int PIC1_RESET_PIN = 4;
const int PIC2_RESET_PIN = 5;
const int PIC3_RESET_PIN = 6;


// TIMER1: every 2ms enter interrupt
volatile int TMR1flag = 1;
const int TMR1TOP = 125;

static char recv = '\0';                          // UART
static int times = 0;                             // count for time game playing(2 min)

static PLAYER p1, p2;                             // player 1, player 2
static bool p1_ready, p2_ready;                   // status for game(all ready that game start)

static bool stopGame;

static bool waitForPIC;                           // wait PIC send '1' 3 times when updating led
static bool pic1ok, pic2ok, pic3ok;
static bool waitCheck = false;
static bool pic1waitok = false;
static bool pic2waitok = false;
static bool pic3waitok = false;

static bool s_Check = false;

static int timeCount = 0;                         // count for running time
static int timePlayer1 = 0;
static int timePlayer2 = 0;

static LED led;                                   // led




void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    // Serial 1 to 3 : USART betweeen self and pic
    Serial1.begin(115200);
    Serial2.begin(115200);
    Serial3.begin(115200);
    // wire setting
    Wire.begin(SLAVE_ADDRESS);
    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    // timer1 setting
    setTimer1();
    pinMode(PIC1_RESET_PIN, OUTPUT);
    pinMode(PIC2_RESET_PIN, OUTPUT);
    pinMode(PIC3_RESET_PIN, OUTPUT);
    digitalWrite(PIC1_RESET_PIN,LOW);
    digitalWrite(PIC2_RESET_PIN,LOW);
    digitalWrite(PIC3_RESET_PIN,LOW);

}

void loop() {
    // put your main code here, to run repeatedly:
    s_Check = false;
    waitForPIC = false;
    waitCheck = false;
    pic1ok = pic2ok = pic3ok = pic1waitok = pic2waitok = pic3waitok = false;
    stopGame = false;
    p1_ready = true;
    p2_ready = true;
    gamestart();
    finalLed(&p1, &p2, &led);
    if(!s_Check) sendCheck();
    // noInterrupts();
}

// TIMER1 interrupt (every 0.002s)
ISR(TIMER1_COMPA_vect) {
    noInterrupts();
    TMR1flag = 1 - TMR1flag;
    detectInput();
    if(p1_ready && p2_ready) {
        if(waitCheck) {
            char pic1, pic2, pic3;
            pic1 = Serial1.read();
            pic2 = Serial2.read();
            pic3 = Serial3.read();
            if(pic1 == 'S') {
                pic1waitok = true;
            }
            if(pic2 == 'S') {
                pic2waitok = true;
            }
            if(pic3 == 'S') {
                pic3waitok = true;
            }
            if(pic1waitok && pic2waitok && pic3waitok)  {
                waitForPIC = true;
                waitCheck = false;
                pic1waitok = false;
                pic2waitok = false;
                pic3waitok = false;
                sendData();
            }
        }else if(!waitForPIC) {
            timeCount++;
            timePlayer1++;
            timePlayer2++;
            // then update game timer
            if(timeCount == 500) {
                // means one second
                timeCount = 0;
                times++;
            }
            // detect player1 not doing thing for one second or not
            if(timePlayer1 == 500) {
                if(!CONTROLLER_autodrop(&p1)) {
                    CONTROLLER_checkbomb(&p1);
                    CONTROLLER_stageUpdate(&p1);
                    CONTROLLER_givebomb(&p2, &p1);
                    if(!CONTROLLER_newTetris(&p1)) {
                        if(!p1.bomb) {
                            stopGame = true;
                        }else{
                            STAGE_cleanAllbomb(p1.stage, p1.bomb);
                            p1.bomb = 0;
                            p2.ko++;
                            CONTROLLER_newTetris(&p1);
                        }
                    }
                }
                stage2led(p1.stage, p2.stage, &led);
                timePlayer1 = 0;
                if(!s_Check) sendCheck();
            }
            // detect player2 not doing thing for one second or not
            if(timePlayer2 == 500) {
                if(!CONTROLLER_autodrop(&p2)) {
                    CONTROLLER_checkbomb(&p2);
                    CONTROLLER_stageUpdate(&p2);
                    CONTROLLER_givebomb(&p1, &p2);
                    if(!CONTROLLER_newTetris(&p2)) {
                        if(!p2.bomb) {
                            stopGame = true;
                        }else{
                            STAGE_cleanAllbomb(p2.stage, p2.bomb);
                            p2.bomb = 0;
                            p1.ko++;
                            CONTROLLER_newTetris(&p2);
                        }
                    }
                }
                stage2led(p1.stage, p2.stage, &led);
                timePlayer2 = 0;
                if(!s_Check) sendCheck();
            }
        }else{
            char pic1, pic2, pic3;
            pic1 = Serial1.read();
            pic2 = Serial2.read();
            pic3 = Serial3.read();
            if(pic1 == 'E') {
                pic1ok = true;
            }
            if(pic2 == 'E') {
                pic2ok = true;
            }
            if(pic3 == 'E') {
                pic3ok = true;
            }
            if(pic1ok && pic2ok && pic3ok)  {
                waitForPIC = false;
                pic1ok = false;
                pic2ok = false;
                pic3ok = false;
                Serial1.write('E');
                Serial2.write('E');
                Serial3.write('E');
                s_Check = false;
            }
        }
    }
    interrupts();

}


void setTimer1() {
    noInterrupts();             // disable interrupt
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= (1 << WGM12);     // CTC mode
    TCCR1B |= (1 << CS12);      // prescaler == 256
    OCR1A = TMR1TOP;
    TCNT1 = 0;                  // counter == 0
    TIMSK1 |= (1 << OCIE1A);    // enable CTC for TIMER1_COMPA_vect
    interrupts();               // enable interrupt
}

void gameinit() {
    times = timePlayer1 = timePlayer2 = 0;
    // player init
    PLAYERinit(&p1);
    PLAYERinit(&p2);
    // stage init
    STAGEinit(p1.stage);
    STAGEinit(p2.stage);
    // led init
    LEDinit(&led);
    return;
}

void gamestart() {
    Serial.write("waiting...\n");
    while(!p1_ready && !p2_ready);
    Serial.write("run !\n");
    gameinit();
    CONTROLLER_newTetris(&p1);
    CONTROLLER_newTetris(&p2);
    while(times < 120 && !stopGame);
    return;
}

void detectInput() {
    /*
     *
     * d, l, r, a, b => player1
     * D, L, R, A, B => player2
     *
     */
    if(p1_ready && p2_ready && !waitForPIC) {
        switch (recv) {
            // player 1
            case 'L':
                CONTROLLER_method(&p1, 'L');
                stage2led(p1.stage, p2.stage, &led);
                if(!s_Check) sendCheck();
                timePlayer1 = 0;
                break;
            case 'R':
                CONTROLLER_method(&p1, 'R');
                stage2led(p1.stage, p2.stage, &led);
                if(!s_Check) sendCheck();
                timePlayer1 = 0;
                break;
            case 'D':
                if(!CONTROLLER_autodrop(&p1)) {
                    CONTROLLER_checkbomb(&p1);
                    CONTROLLER_stageUpdate(&p1);
                    CONTROLLER_givebomb(&p2, &p1);
                    if(!CONTROLLER_newTetris(&p1)) {
                        if(!p1.bomb) {
                            stopGame = true;
                        }else{
                            STAGE_cleanAllbomb(p1.stage, p1.bomb);
                            p1.bomb = 0;
                            p2.ko++;
                        }
                    }
                }
                stage2led(p1.stage, p2.stage, &led);
                if(!s_Check) sendCheck();
                timePlayer1 = 0;
                break;
            case 'A':
                CONTROLLER_method(&p1, 'F');
                stage2led(p1.stage, p2.stage, &led);
                if(!s_Check) sendCheck();
                timePlayer1 = 480;
                break;
            case 'B':
                CONTROLLER_method(&p1, 'r');
                stage2led(p1.stage, p2.stage, &led);
                if(!s_Check) sendCheck();
                timePlayer1 = 0;
                break;
                // player 2
            case 'l':
                CONTROLLER_method(&p2, 'L');
                stage2led(p1.stage, p2.stage, &led);
                if(!s_Check) sendCheck();
                timePlayer2 = 0;
                break;
            case 'r':
                CONTROLLER_method(&p2, 'R');
                stage2led(p1.stage, p2.stage, &led);
                if(!s_Check) sendCheck();
                timePlayer2 = 0;
                break;
            case 'd':
                if(!CONTROLLER_autodrop(&p2)) {
                    CONTROLLER_checkbomb(&p2);
                    CONTROLLER_stageUpdate(&p2);
                    CONTROLLER_givebomb(&p1, &p2);
                    if(!CONTROLLER_newTetris(&p2)) {
                        if(!p2.bomb) {
                            stopGame = true;
                        }else{
                            STAGE_cleanAllbomb(p2.stage, p2.bomb);
                            p2.bomb = 0;
                            p1.ko++;
                        }
                    }
                }
                stage2led(p1.stage, p2.stage, &led);
                if(!s_Check) sendCheck();
                timePlayer2 = 0;
                break;
            case 'a':
                CONTROLLER_method(&p2, 'F');
                stage2led(p1.stage, p2.stage, &led);
                if(!s_Check) sendCheck();
                timePlayer2 = 480;
                break;
            case 'b':
                CONTROLLER_method(&p2, 'r');
                stage2led(p1.stage, p2.stage, &led);
                if(!s_Check) sendCheck();
                timePlayer2 = 0;
                break;

            default: break;
        }
    }else{
        switch(recv) {
            case 'K':
                p1_ready = true;
                Serial.write("p1 go\r\n");
                break;
            case 'k':
                p2_ready = true;
                Serial.write("p2 go\r\n");
                break;
        }
    }
    recv = '\0';
}

// callback for received data

void receiveData(int byteCount) {
    if (Wire.available()) {
        recv = Wire.read();
    }
}

void sendCheck() {
    Serial1.write('S');
    Serial2.write('S');
    Serial3.write('S');
    s_Check = true;
    waitCheck = true;
}

void sendData () {
    // send data to first row
    for(int i=0; i<24; ++i) { Serial1.write(led.red[i/3][i%3]); }
    for(int i=0; i<24; ++i) { Serial1.write(led.blue[i/3][i%3]); }
    // send data to second row
    for(int i=24; i<48; ++i) { Serial2.write(led.red[i/3][i%3]); }
    for(int i=24; i<48; ++i) { Serial2.write(led.blue[i/3][i%3]); }
    // send data to third row
    for(int i=48; i<72; ++i) { Serial3.write(led.red[i/3][i%3]); }
    for(int i=48; i<72; ++i) { Serial3.write(led.blue[i/3][i%3]); }
    waitForPIC = true;
    return;
}
