#ifndef BLASTPOWERUP_H_ 
#define BLASTPOWERUP_H_ 
 
#include "Global.h" 
#include <stdlib.h> 
#include <time.h> 
extern Global g; 
 
class BlastPowerup { 
    public: 
        float pos[2]; 
 
        BlastPowerup () { 
 
            srand(time(NULL)); 
            pos[0] = 0.0; 
            pos[1] = 0.0; 
        } 
}; 
 
#endif 
~         
