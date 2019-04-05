#ifndef GLOBAL_H_
#define GLOBAL_H_
#define rnd() (((double)rand())/(double)RAND_MAX)
typedef double Vec[3];

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
        GLuint namekTexture;
        GLuint saibaTexture;

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
};




#endif
