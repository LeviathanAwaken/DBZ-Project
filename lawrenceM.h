#ifndef LAWRENCEM_H_
#define LAWRENCEM_H_

// #include "Global.h"
#include <bitset>
#include <stdlib.h>
using namespace std;

/* health object class */
//abcdefg - #
//1111110 - 0
//0110000 - 1
//1101101 - 2
//1111001 - 3
//0110011 - 4
//1011011 - 5
//0011111 - 6
//1110000 - 7
//1111111 - 8
//1110011 - 9
struct healthSSD
{

    bitset<7> screen;
    bitset<7> screen2;
    healthSSD()
    {
        screen = bitset<7>("1111001");
        screen2 = bitset<7>("1111110");
    }
    void updateHealthCounter(int health)
    {
        switch (health)
        {
        case 1:
            screen = bitset<7>("0110000");
            break;
        case 2:
            screen = bitset<7>("1101101");
            break;
        case 3:
            screen = bitset<7>("1111001");
            break;
        case 4:
            screen = bitset<7>("0110011");
            break;
        case 5:
            screen = bitset<7>("1011011");
            break;
        case 6:
            screen = bitset<7>("0011111");
            break;
        case 7:
            screen = bitset<7>("1110000");
            break;
        case 8:
            screen = bitset<7>("1111111");
            break;
        case 9:
            screen = bitset<7>("1110011");
            break;
        default:
            screen = bitset<7>("1001111");
            break;
        }
    }
    void updateDBallCounter(int dBalls)
    {
        switch (dBalls)
        {
        case 1:
            screen2 = bitset<7>("0110000");
            break;
        case 2:
            screen2 = bitset<7>("1101101");
            break;
        case 3:
            screen2 = bitset<7>("1111001");
            break;
        case 4:
            screen2 = bitset<7>("0110011");
            break;
        case 5:
            screen2 = bitset<7>("1011011");
            break;
        case 6:
            screen2 = bitset<7>("0011111");
            break;
        case 7:
            screen2 = bitset<7>("1110000");
            break;
        case 8:
            screen = bitset<7>("1111111");
            break;
        case 9:
            screen = bitset<7>("1110011");
            break;
        default:
            screen = bitset<7>("1111001");
            break;
        }
    }
    void renderHealthSSD()
    {
        if (screen[6])
        { //a
            glBegin(GL_POLYGON);
            // glVertex2f(0.0f, 100.0f);
            // glVertex2f(0.0f, 110.0f);
            // glVertex2f(40.0f, 110.0f);
            // glVertex2f(40.0f, 100.0f);
            glVertex2f(0.0f, 50.0f);
            glVertex2f(0.0f, 55.0f);
            glVertex2f(20.0f, 55.0f);
            glVertex2f(20.0f, 50.0f);
            glEnd();
        }
        if (screen[5])
        { //b
            glBegin(GL_POLYGON);
            // glVertex2f(30.0f, 60.0f);
            // glVertex2f(30.0f, 100.0f);
            // glVertex2f(40.0f, 100.0f);
            // glVertex2f(40.0f, 60.0f);
            glVertex2f(15.0f, 30.0f);
            glVertex2f(15.0f, 50.0f);
            glVertex2f(20.0f, 50.0f);
            glVertex2f(20.0f, 30.0f);
            glEnd();
        }
        if (screen[4])
        { //c
            glBegin(GL_POLYGON);
            // glVertex2f(30.0f, 10.0f);
            // glVertex2f(30.0f, 50.0f);
            // glVertex2f(40.0f, 50.0f);
            // glVertex2f(40.0f, 10.0f);
            glVertex2f(15.0f, 5.0f);
            glVertex2f(15.0f, 25.0f);
            glVertex2f(20.0f, 25.0f);
            glVertex2f(20.0f, 5.0f);
            glEnd();
        }
        if (screen[3])
        { //d
            glBegin(GL_POLYGON);
            // glVertex2f(0.0f, 0.0f);
            // glVertex2f(0.0f, 10.0f);
            // glVertex2f(40.0f, 10.0f);
            // glVertex2f(40.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0f, 5.0f);
            glVertex2f(20.0f, 5.0f);
            glVertex2f(20.0f, 0.0f);
            glEnd();
        }
        if (screen[2])
        { //e
            glBegin(GL_POLYGON);
            // glVertex2f(0.0f, 10.0f);
            // glVertex2f(0.0f, 50.0f);
            // glVertex2f(10.0f, 50.0f);
            // glVertex2f(10.0f, 10.0f);
            glVertex2f(0.0f, 5.0f);
            glVertex2f(0.0f, 25.0f);
            glVertex2f(5.0f, 25.0f);
            glVertex2f(5.0f, 5.0f);
            glEnd();
        }
        if (screen[1])
        { //f
            glBegin(GL_POLYGON);
            // glVertex2f(0.0f, 60.0f);
            // glVertex2f(0.0f, 100.0f);
            // glVertex2f(10.0f, 100.0f);
            // glVertex2f(10.0f, 60.0f);
            glVertex2f(0.0f, 30.0f);
            glVertex2f(0.0f, 50.0f);
            glVertex2f(5.0f, 50.0f);
            glVertex2f(5.0f, 30.0f);
            glEnd();
        }
        if (screen[0])
        { //g
            glBegin(GL_POLYGON);
            // glVertex2f(0.0f, 50.0f);
            // glVertex2f(0.0f, 60.0f);
            // glVertex2f(40.0f, 60.0f);
            // glVertex2f(40.0f, 50.0f);
            glVertex2f(0.0f, 25.0f);
            glVertex2f(0.0f, 30.0f);
            glVertex2f(20.0f, 30.0f);
            glVertex2f(20.0f, 25.0f);
            glEnd();
        }
    }
    void renderDBallCounterSSD()
    {
        if (screen2[6])
        { //a
            glBegin(GL_POLYGON);
            // glVertex2f(0.0f, 100.0f);
            // glVertex2f(0.0f, 110.0f);
            // glVertex2f(40.0f, 110.0f);
            // glVertex2f(40.0f, 100.0f);
            glVertex2f(0.0f, 50.0f);
            glVertex2f(0.0f, 55.0f);
            glVertex2f(20.0f, 55.0f);
            glVertex2f(20.0f, 50.0f);
            glEnd();
        }
        if (screen2[5])
        { //b
            glBegin(GL_POLYGON);
            // glVertex2f(30.0f, 60.0f);
            // glVertex2f(30.0f, 100.0f);
            // glVertex2f(40.0f, 100.0f);
            // glVertex2f(40.0f, 60.0f);
            glVertex2f(15.0f, 30.0f);
            glVertex2f(15.0f, 50.0f);
            glVertex2f(20.0f, 50.0f);
            glVertex2f(20.0f, 30.0f);
            glEnd();
        }
        if (screen2[4])
        { //c
            glBegin(GL_POLYGON);
            // glVertex2f(30.0f, 10.0f);
            // glVertex2f(30.0f, 50.0f);
            // glVertex2f(40.0f, 50.0f);
            // glVertex2f(40.0f, 10.0f);
            glVertex2f(15.0f, 5.0f);
            glVertex2f(15.0f, 25.0f);
            glVertex2f(20.0f, 25.0f);
            glVertex2f(20.0f, 5.0f);
            glEnd();
        }
        if (screen2[3])
        { //d
            glBegin(GL_POLYGON);
            // glVertex2f(0.0f, 0.0f);
            // glVertex2f(0.0f, 10.0f);
            // glVertex2f(40.0f, 10.0f);
            // glVertex2f(40.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0f, 5.0f);
            glVertex2f(20.0f, 5.0f);
            glVertex2f(20.0f, 0.0f);
            glEnd();
        }
        if (screen2[2])
        { //e
            glBegin(GL_POLYGON);
            // glVertex2f(0.0f, 10.0f);
            // glVertex2f(0.0f, 50.0f);
            // glVertex2f(10.0f, 50.0f);
            // glVertex2f(10.0f, 10.0f);
            glVertex2f(0.0f, 5.0f);
            glVertex2f(0.0f, 25.0f);
            glVertex2f(5.0f, 25.0f);
            glVertex2f(5.0f, 5.0f);
            glEnd();
        }
        if (screen2[1])
        { //f
            glBegin(GL_POLYGON);
            // glVertex2f(0.0f, 60.0f);
            // glVertex2f(0.0f, 100.0f);
            // glVertex2f(10.0f, 100.0f);
            // glVertex2f(10.0f, 60.0f);
            glVertex2f(0.0f, 30.0f);
            glVertex2f(0.0f, 50.0f);
            glVertex2f(5.0f, 50.0f);
            glVertex2f(5.0f, 30.0f);
            glEnd();
        }
        if (screen2[0])
        { //g
            glBegin(GL_POLYGON);
            // glVertex2f(0.0f, 50.0f);
            // glVertex2f(0.0f, 60.0f);
            // glVertex2f(40.0f, 60.0f);
            // glVertex2f(40.0f, 50.0f);
            glVertex2f(0.0f, 25.0f);
            glVertex2f(0.0f, 30.0f);
            glVertex2f(20.0f, 30.0f);
            glVertex2f(20.0f, 25.0f);
            glEnd();
        }
    }
};

void renderControls();
void showLawrenceText(int, int);
void showLawrencePicture(int, int, GLuint);
void showLawrence(int, int, GLuint);
void setBackground(int, int, GLuint);
void newGame();
int acceptGameState(int);
void checkKeysLost();
void checkKeysMainMenu();
void checkKeysPauseMenu();
void renderMainMenuBackground(int, int, GLuint);
void renderMainMenu();
void renderPauseMenu();
void renderControls();
void renderDeath();

#endif
