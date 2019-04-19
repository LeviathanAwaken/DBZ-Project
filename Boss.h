#ifndef BOSS_H_
#define BOSS_H_

#include "Global.h"
#include <stdlib.h>
#include <time.h>
extern Global g;


//-------------------Enemy class---------------------------------
class Boss {
public:
    
    float pos[3];
    int pattern;
    float wavepos;
    int wavefreq;
    int waveamp;
    int xSpeed;
    int eHealth;
    

    
    Boss () {
     
     srand(time(NULL));
     //pattern = 1;
     eHealth = 20;
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