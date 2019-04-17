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

    
    Enemy () {
     
     srand(time(NULL));
     pattern = 1;
     wavepos = 0.0;
     pos[0] = 0.0;	
     pos[1] = 0.0;	
     pos[2] = 0.0;
    }
};

#endif