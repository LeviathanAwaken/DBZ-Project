#ifndef POWERUPS_H_
#define POWERUPS_H_

#include "Global.h"
#include <stdlib.h>
#include <time.h>
extern Global g;

class Powerups {
    public:
	float pos[1];
	int pattern;
	float wavepos;

	Powerups () {

	    srand(time(NULL));
	    pattern = 1;
	    wavepos = 0.0;
	    pos[0] = 0.0;
	}
};

#endif
