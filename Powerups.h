#ifndef POWERUPS_H_
#define POWERUPS_H_

#include "Global.h"
#include <stdlib.h>
#include <time.h>
extern Global g;

class Powerups {
	public:
	float pos[2];

	Powerups () {

		//srand(time(NULL));
		pos[0] = 0.0;
		pos[1] = 0.0;
	}
};

#endif
