#ifndef LAWRENCEM_H_
#define LAWRENCEM_H_

#include "Global.h"
#include <stdlib.h>

/* health object class */

class Health {
    public:
    float pos[2]; // p[0] = x, p[1] = y;
    float colors[3]; 

    Health() {
        pos[0] = 0.0;
        pos[1] = 0.0;
        colors[0] = 0.0;
        colors[1] = 0.0;
        colors[2] = 0.0;
    }
};

void renderControls();
void showLawrenceText(int, int);
void showLawrencePicture (int, int, GLuint);
void showLawrence(int,int,GLuint);
void setBackground(int,int, GLuint);
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




