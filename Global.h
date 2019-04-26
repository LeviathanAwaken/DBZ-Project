#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <cstdlib>
#define rnd() (((double)rand())/(double)RAND_MAX)
typedef double Vec[3];

//game states
#define MAINMENU 0
#define INGAME 1
#define PAUSEMENU 2
#define DEATH 3

//menu options
#define NEWGAME 0
#define EXIT 1
#define RESUMEGAME 2

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
		GLuint powerupTexture;
		GLuint bossTexture;
		GLuint finalFormLogoTexture;
		GLuint explosionTexture;
		GLuint ss3Texture;
		GLuint ssbTexture;
		GLuint braceTexture;
		GLuint explosion2Texture;
		GLuint explosion3Texture;

		Vec box[20];
		Global() {
			done=0;
			xres=1920;
			yres=1080;
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
