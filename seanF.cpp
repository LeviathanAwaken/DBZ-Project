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
const int MAX_ENEM = 10;

Enemy *enemyRef[MAX_ENEM];
int limiter = 0;

void kiCollision(int);
void kiHandler(int);
void gokuRender();
void gokuCollision();

class Protag {
	public:
		GLuint pic;
		float pos[2];
		float vel[2];
		int height;
		int width;
		int health;
} goku;

//Class to track x and y positions of the blasts being sent by the character.
class kiBlast {
	public:
		GLuint image;
		int kiTracker[MAX_KI][2];
		int kiVel;
} ki;

void enemyReference(Enemy* enem)
{
	enemyRef[limiter] = enem;
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

void gokuInit()
{
	goku.vel[0] = 0;
	goku.vel[1] = 0;
	goku.height = 70;
	goku.width = 100;
	goku.pos[0] = g.xres / 2 - (goku.width / 2);
	goku.pos[1] = g.yres / 2 - (goku.height / 2);
	goku.health = 3;
}

//Generalized initializer for the file, called in the main file.
void sInit(GLuint gok, GLuint image)
{
	goku.pic = gok;
	ki.image = image;
	gokuInit();
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

void velUpd(int key)
{
	switch (key) {
		case 0:
			goku.vel[0] -= .5;
			break;
		case 1:
			goku.vel[0] += .5;
			break;
		case 2:
			goku.vel[1] += .5;
			break;
		case 3:
			goku.vel[1] -= .5;
			break;
	}
}

void gokuMove()
{
	if ((goku.pos[0] > 0 && goku.vel[0] < 0)
	|| (goku.pos[0] < (g.xres - goku.width) && goku.vel[0] > 0))
		goku.pos[0] += goku.vel[0];
	else
		goku.vel[0] = 0;
	if ((goku.pos[1] > 0 && goku.vel[1] < 0)
	|| (goku.pos[1] < (g.yres - goku.height) && goku.vel[1] > 0))
		goku.pos[1] += goku.vel[1];
	else
		goku.vel[1] = 0;
	gokuCollision();
}

void sRender()
{
	gokuRender();
	kiHandler(1);
}

void gokuRender()
{
	glPushMatrix();

	glTranslatef(goku.pos[0], goku.pos[1], 0);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, g.walkTexture);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);

	// CHANGED
	int ix = 0;
	int iy = 0;
	if (g.walkFrame >= 8)
		iy = 1;

	float tx = (float)ix;
	float ty = (float)iy;

	glBegin(GL_QUADS);
		glTexCoord2f(tx+1, ty+1); 	glVertex2i(0, 0);
		glTexCoord2f(tx+1, ty);    	glVertex2i(0, goku.height);
		glTexCoord2f(tx, ty);    	glVertex2i(goku.width, goku.height);
		glTexCoord2f(tx, ty+1); 	glVertex2i(goku.width, 0);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
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
void launchKi()
{
	int kiNum = kiLimitCheck();
	if (kiNum != -1) {
		ki.kiTracker[kiNum][0] = goku.pos[0] + 50;
		ki.kiTracker[kiNum][1] = goku.pos[1];
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
	if (ki.kiTracker[kiID][0] > g.xres) {
		kiFree(kiID);
	} else {
		ki.kiTracker[kiID][0] += ki.kiVel;
	}
	kiCollision(kiID);
}

//Handles graphics for the kiBlasts.
void kiRender(int kiID)
{
	float h = 40.0;
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
	glTexCoord2f(tx+1, ty+1); 	glVertex2i(0, 0);
	glTexCoord2f(tx+1, ty);    	glVertex2i(0, h);
	glTexCoord2f(tx, ty);    	glVertex2i(w, h);
	glTexCoord2f(tx, ty+1); 	glVertex2i(w, 0);
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
	for (int i = 0; i < MAX_ENEM; i++) {
		//printf("%f\t%d\t%d\n", enemyRef[i]->pos[0], ki.kiTracker[kiRef][0], g.xres);
		bool xColl = enemyRef[i]->pos[0] + 70 >= ki.kiTracker[kiRef][0]
			&& ki.kiTracker[kiRef][0] + 15 >= enemyRef[i]->pos[0];
		bool yColl = enemyRef[i]->pos[1] + 50 >= ki.kiTracker[kiRef][1]
			&& ki.kiTracker[kiRef][1] + 30 >= enemyRef[i]->pos[1];
		if (xColl && yColl) {
			kiFree(kiRef);
			//INSERT ENEMY DELETION HERE
			break;
		}
	}
}

//Collision checking for the main character and the enemies.
void gokuCollision()
{
	for (int i = 0; i < MAX_ENEM; i++) {
		bool xColl = enemyRef[i]->pos[0] + 70 >= goku.pos[0]
			&& goku.pos[0] + goku.width >= enemyRef[i]->pos[0];
		bool yColl = enemyRef[i]->pos[1] + 50 >= goku.pos[1]
			&& goku.pos[1] + goku.height >= enemyRef[i]->pos[1];
		if (xColl && yColl) {
			goku.health--;
			//UPDATE HERE, REGISTERS TOO MANY HITS RIGHT NOW
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

//Sound handler, not functioning yet.
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
