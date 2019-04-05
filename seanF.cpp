//3350
//Program:  seanF.cpp
//Author:   Sean Fontes
//Date:     2-14-19

/*
 * Description:
 * File currently holds the main logic for firing projectiles from the
 * character's position using the 'k' key logic from the main file. All
 * logic for graphics, positionally, and limitting-wise, is handled by
 * functions called from this file. The file also features a handler
 * function that calls the correct functions within the file depending
 * on the value passed from the walk file.
 */
#include <GL/glx.h>
#include "fonts.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <vector>
#include "Enemy.h"
#ifdef SOUND
	#include </usr/include/AL/alut.h>
	#include <unistd.h>
#endif

const int MAX_KI = 10;
const int UNASSIGN = -5000;
const int COLLISION = 2;
Enemy *enemyRef[3];
int limiter = 0;
void kiCollision(int);

//File tracks x and y resolutions to prevent repassing them in functions.
class Glob {
	public:
		int xres, yres;
} glob;

//Class to track x and y positions of the blasts being sent by the character.
class kiBlast {
	public:
		GLuint image;
		int kiTracker[MAX_KI][2];
		int kiVel;
} ki;

void enemyReference(Enemy* enem)
{
	switch (limiter) {
		case 0:
			enemyRef[0] = enem;
			break;
		case 1:
			enemyRef[1] = enem;
			break;
		case 2:
			enemyRef[2] = enem;
			break;
	}
	limiter++;
}

//Initializes the kiClass for use.
void kiInit()
{
	ki.kiVel = 2;
	for (int i = 0; i < MAX_KI; i++) {
		ki.kiTracker[i][0] = UNASSIGN;
		ki.kiTracker[i][1] = UNASSIGN;
	}
}

//Generalized initializer for the file, called in the main file.
void sInit(GLuint image, int xres, int yres)
{
	ki.image= image;
	glob.xres = xres;
	glob.yres = yres;
	kiInit();

}

void showText(int x, int y)
{
	Rect r;
	unsigned int c = 0x00ffff44;
	r.bot = y-20;
	r.left = x;
	r.center = 0;
	ggprint8b(&r, 16, c, "Sean Fontes");
}

void showPic(int x, int y, GLuint textInt)
{
	glBindTexture(GL_TEXTURE_2D, textInt);
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(x+100, y+100);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(x+100, y+250);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(x+250, y+250);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(x+250, y+100);
	glEnd();
}

void showSean(int x, int y, GLuint textInt)
{
	showPic(0, 0, textInt);
	showText(x+100, y+100);
}

//Checks ki position against window bounds.
int kiLimitCheck()
{
	for (int i = 0; i < MAX_KI; i++) {
		if (ki.kiTracker[i][0] == UNASSIGN && ki.kiTracker[i][1] == UNASSIGN)
			return i;
	}
	return -1;
}

//Creates a kiBlast, if it's not at max.
void launchKi(int x, int y)
{
	int kiNum = kiLimitCheck();
	if (kiNum != -1) {
		ki.kiTracker[kiNum][0] = x;
		ki.kiTracker[kiNum][1] = y;
	}
}

//Destroys the kiBlast, and 'unassigns' it.
void kiFree(int kiID)
{
	ki.kiTracker[kiID][0] = UNASSIGN;
	ki.kiTracker[kiID][1] = UNASSIGN;
}

//Updates the position of the kiBlast.
void kiMove(int kiID)
{
	if (ki.kiTracker[kiID][0] > glob.xres/2) {
		kiFree(kiID);
	} else {
		ki.kiTracker[kiID][0] += ki.kiVel;
	}
	kiCollision(kiID);
}

//Handles graphics for the kiBlasts.
void kiRender(int kiID)
{
	float cx = glob.xres/2.0;
	float cy = glob.yres/2.0;
	float h = 15.0;
	float w = h*2;
	glPushMatrix();

	glTranslatef(ki.kiTracker[kiID][0], ki.kiTracker[kiID][1], 0);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, ki.image);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);

	float tx = 0.0;
	float ty = 0.0;
	glBegin(GL_QUADS);
	glTexCoord2f(tx+1,      ty+1); glVertex2i(cx-w, cy-h);
	glTexCoord2f(tx+1,      ty);    glVertex2i(cx-w, cy+h);
	glTexCoord2f(tx, ty);    glVertex2i(cx+w, cy+h);
	glTexCoord2f(tx, ty+1); glVertex2i(cx+w, cy-h);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
}

/*
 * This function currently is using a placeholder for the input
 * It will eventually take in an enemy class from Drake's file, and then
 * construct a rectangle around the enemy and check for collisions
 * against that.
 * Enemy& enem
*/
void kiCollision(int kiRef)
{
	for (int i = 0; i < 3; i++) {
		printf("%f\t%d\t%d\n", enemyRef[i]->pos[0], ki.kiTracker[kiRef][0], glob.xres);
		bool xColl = enemyRef[i]->pos[0] + 70 >= ki.kiTracker[kiRef][0]
			&& ki.kiTracker[kiRef][0] + 15 >= enemyRef[i]->pos[0];
		bool yColl = enemyRef[i]->pos[1] + 50 >= ki.kiTracker[kiRef][1]
			&& ki.kiTracker[kiRef][1] + 30 >= enemyRef[i]->pos[1];
		if (xColl && yColl) {
			kiFree(kiRef);
			break;
		}
	}
}

//Handler function to prevent repetitive code of parsing through the kiBlasts.
void kiHandler(int type)
{
	for (int i = 0; i < MAX_KI; i++) {
		if (ki.kiTracker[i][0] != UNASSIGN &&
			ki.kiTracker[i][1] != UNASSIGN) {
			if (type == 0) {
				kiMove(i);
			} else if (type == 1) {
				kiRender(i);
			}
		}
	}
}

void soundHandle()
{
	#ifdef SOUND
		alutInit(0, NULL);
		if (alGetError() != AL_NO_ERROR) {
			printf("Error: alutInit()\n");
		}
		alGetError();

		float vec[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
		alListenerfv(AL_ORIENTATION, vec);
		alListenerf(AL_GAIN, 1.0f);
		ALuint albuffer;
		albuffer = alutCreateBufferFromFile("./sounds/supa sayain.mp3");

		ALuint alSource;

		alGenSources(1, &alSource);
		alSourcei(alSource, AL_BUFFER, albuffer);
		alSourcef(alSource, AL_GAIN, 1.0f);
		alSourcef(alSource, AL_PITCH, 1.0f);
		alSourcei(alSource, AL_LOOPING, AL_TRUE);
		if (alGetError() != AL_NO_ERROR) {
			printf("Error: setting source\n");
		}
		for (int i = 0; i < 4; i++) {
			alSourcePlay(alSource);
			usleep(250000);
		}

		alDeleteSources(1, &alSource);
		alDeleteBuffers(1, &albuffer);
		ALCcontext *Context = alcGetCurrentContext();
		ALCdevice *Device = alcGetContextsDevice(Context);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(Context);
		alcCloseDevice(Device);
	#endif
}
