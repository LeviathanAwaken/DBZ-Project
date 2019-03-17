#include "Global.h"
#include <X11/keysym.h>
 
 Global::Global() {
            done=0;
            xres=800;
            yres=600;
            score = 0;
            memset(keys, 0, 50000);
            //CHANGED - back scroll starts on launch now
            walk=0;
            walkFrame=1;
            creditFlag = 0;
            startFlag = 1;
            pauseFlag = 0;
            paused = false;
            delay = 0.09;
            for (int i=0; i<20; i++) {
                box[i][0] = rnd() * xres;
                box[i][1] = rnd() * (yres-220) + 220.0;
                box[i][2] = 0.0;
            }
 }