#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)     ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]

//game states
#define MAINMENU 0
#define INGAME 1
#define PAUSEMENU 2
#define DEATH 3

//menu options
#define NEWGAME 0
#define LEADERBOARD 1
#define EXIT 2
#define RESUMEGAME 3

//global class
class Global {
    public:
        int done;
        int xres, yres;
        int score;
        int walk;
        int walkFrame;
        int creditFlag;
        int startFlag;
        int pauseFlag;
        int spriteSheetIndex;
        bool paused;
        double delay;
        char keys[65536];
        GLuint walkTexture;
        GLuint cloudTexture;
        GLuint seanTexture;
        GLuint lawrenceTexture;
        GLuint joshTexture;
        GLuint drakeTexture;
        GLuint juanTexture;
        GLuint kiTexture;
        GLuint enemyTexture;
        GLuint namekTexture;

        Vec box[20];
        Global() {
            done=0;
            xres=800;
            yres=600;
        score = 0;
            memset(keys, 0, 65536);
            //CHANGED - back scroll starts on launch now
            walk=1;
            walkFrame=0;
            creditFlag = 0;
            startFlag = 0;
            pauseFlag = 0;
        paused = true;
            delay = 0.09;
            for (int i=0; i<20; i++) {
                box[i][0] = rnd() * xres;
                box[i][1] = rnd() * (yres-220) + 220.0;
                box[i][2] = 0.0;
            }
        }
} 

#endif