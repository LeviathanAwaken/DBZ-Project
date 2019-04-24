#ifndef ENEMY_H_
#define ENEMY_H_

#include "Global.h"
#include <stdlib.h>
#include <time.h>
extern Global g;


//-------------------Enemy class---------------------------------
class Enemy {
public:
    
    float pos[3];
    int pattern;
    float wavepos;
    int wavefreq;
    int waveamp;
    int xSpeed;
    int eHealth;
    bool isRendered;
    

    
    Enemy () {
     
     srand(time(NULL));
     isRendered = false;
     pattern = 1;
     eHealth = 2;
     wavepos = 0.0;
     xSpeed = 0;
     wavefreq = 0;
     waveamp = 0;
     pos[0] = 0.0;	
     pos[1] = 0.0;	
     pos[2] = 0.0;
    }
};

#endif