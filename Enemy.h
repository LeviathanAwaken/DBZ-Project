#ifndef ENEMY_H_
#define ENEMY_H_

#include "Global.h"
extern Global g;


//-------------------Enemy class---------------------------------
class Enemy {
public:
    
    float pos[3];

    
    Enemy () {
     
     srand(time(0));

     pos[0] = g.xres;
     pos[1] = (rand() % (g.yres-100) + 1);
     pos[2] = 0.0;
    }
};

#endif