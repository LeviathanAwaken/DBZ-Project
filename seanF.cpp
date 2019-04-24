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
#include "Powerups.h"
#include "Boss.h"
#ifdef SOUND
	#include </usr/include/AL/alut.h>
	#include <unistd.h>
#endif

const int MAX_KI = 10;
const int MAX_BRACE = 20;
const int UNASSIGN = -5000;
const int COLLISION = 2;
const int MAX_ENEM = 10;

Enemy *enemyRef[MAX_ENEM];
Boss *finBoss;
Powerups *powRef[2];
int elimiter = 0;
int plimiter = 0;

//Prototypes and extern function calls
void kiCollision(int);
void kiHandler(int);
void braceHandler(int);
void bossCollision(int);
void gokuRender();
void gokuCollision();
void healthCheck();
bool gokuBounds(int);
void gokuPower();
extern void detection(int);
extern void bossDetection();

//Class encompassing the main character's position and other attributes.
class Protag {
	public:
		GLuint pics[3];
		int currentPic;
		float pos[2];
		//float vel[2];
		int height;
		int width;
		int health;
		float moveS;
} goku;

//Class to track x and y positions of the blasts being sent by the character.
class kiBlast {
	public:
		GLuint image;
		int kiTracker[MAX_KI][2];
		int kiVel;
} ki;


class bossProjectile {
	public:
		GLuint image;
		int bossTracker[MAX_BRACE][2];
		int bossXVel;
		int bossYVel[MAX_BRACE];
} brace;


//Establishes a poiner to enemies being handled in Drake's file.
void enemyReference(Enemy* enem)
{
	enemyRef[elimiter] = enem;
	elimiter++;
}

void bossReference(Boss* bossIn)
{
	finBoss = bossIn;
}

//Establishes a pointer to powerups being handled in Josh's file.
void powerReference(Powerups* power)
{
	powRef[plimiter] = power;
	plimiter++;
}

//Initializes the kiClass for use.
void kiInit()
{
	ki.kiVel = 5;
	for (int i = 0; i < MAX_KI; i++) {
		ki.kiTracker[i][0] = UNASSIGN;
		ki.kiTracker[i][1] = UNASSIGN;
	}
}

//Sets up the starting values for the main character.
void gokuInit()
{
	//goku.vel[0] = 0;
	//goku.vel[1] = 0;
	goku.height = 70;
	goku.width = 100;
	goku.pos[0] = g.xres / 2 - (goku.width / 2);
	goku.pos[1] = g.yres / 2 - (goku.height / 2);
	goku.health = 3;
	goku.moveS = 3.5;
	goku.currentPic = 0;

}

void braceInit()
{
	brace.bossXVel = -6;
	for (int i = 0; i < MAX_BRACE; i++) {
		brace.bossTracker[i][0] = UNASSIGN;
		brace.bossTracker[i][1] = UNASSIGN;
	}

}

//Generalized initializer for the file, called in the main file.
void sInit(GLuint gok, GLuint gok2, GLuint gok3)
{
	goku.pics[0] = gok;
	goku.pics[1] = gok2;
	goku.pics[2] = gok3;
	gokuInit();
	kiInit();
	braceInit();
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

/*void velUpd(int key)
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
}*/

void gokuIMove(int key)
{
	bool bounds = gokuBounds(key);
	switch (key) {
		case 0:
			if (bounds)
				goku.pos[0] -= goku.moveS;
			else
				goku.pos[0] = 0;
			break;
		case 1:
			if (bounds)
				goku.pos[0] += goku.moveS;
			else
				goku.pos[0] = g.xres - goku.width;
			break;
		case 2:
			if (bounds)
				goku.pos[1] += goku.moveS;
			else
				goku.pos[1] = g.yres - goku.height;
			break;
		case 3:
			if (bounds)
				goku.pos[1] -= goku.moveS;
			else
				goku.pos[1] = 0;
			break;
	}
	gokuCollision();
	gokuPower();
}

bool gokuBounds(int dir)
{
	if (dir == 0) {
		return (goku.pos[0] > 0 && goku.pos[0] - goku.moveS > 0);
	} else if (dir == 1) {
		return (goku.pos[0] < g.xres - goku.width
				&& goku.pos[0] + goku.moveS < g.xres - goku.width);
	} else if (dir == 2) {
		return (goku.pos[1] <= g.yres - goku.height
				&& goku.pos[1] + goku.moveS <= (g.yres + 30) - goku.width);
	} else if (dir == 3) {
		return (goku.pos[1] >= 0 && goku.pos[1] - goku.moveS >= 0);
	} else {
		return false;
	}
}

/*void gokuMove()
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
}*/

void sRender()
{
	gokuRender();
	kiHandler(1);
	braceHandler(1);
}

void gokuRender()
{
	glPushMatrix();

	glTranslatef(goku.pos[0], goku.pos[1], 0);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, goku.pics[goku.currentPic]);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);

	float tx = 0.0;
	float ty = 0.0;
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
	bossCollision(kiID);
}

//Handles graphics for the kiBlasts.
void kiRender(int kiID)
{
	float h = 40.0;
	float w = h*2;
	glPushMatrix();

	glTranslatef(ki.kiTracker[kiID][0], ki.kiTracker[kiID][1], 0);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, g.kiTexture);


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

//Creates a kiBlast, if it's not at max.
void launchBrace(int id)
{
	brace.bossTracker[id][0] = finBoss->pos[0] - 20;
	brace.bossTracker[id][1] = finBoss->pos[1];
	srand(time(0));
	brace.bossYVel[id] = (rand() % 5) * ((rand() % 2 == 1) ? 1 : -1);
}

//Destroys the kiBlast, and 'unassigns' it.
void braceFree(int braceID)
{
	brace.bossTracker[braceID][0] = UNASSIGN;
	brace.bossTracker[braceID][1] = UNASSIGN;
}

//Updates the position of the kiBlast.
void braceMove(int braceID)
{
	if (brace.bossTracker[braceID][0] < 0 ||
		brace.bossTracker[braceID][1] > g.yres ||
		brace.bossTracker[braceID][1] < 0) {
		braceFree(braceID);
	} else {
		brace.bossTracker[braceID][0] += brace.bossXVel;
		brace.bossTracker[braceID][1] += brace.bossYVel[braceID];
	}
}

void braceRender(int braceID)
{
	float h = 40.0;
	float w = h/2;
	glPushMatrix();

	glTranslatef(brace.bossTracker[braceID][0],
		brace.bossTracker[braceID][1], 0);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, g.braceTexture);


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

//Collision checking between kiblast and enemies.
void kiCollision(int kiRef)
{
	for (int i = 0; i < MAX_ENEM; i++) {
		bool xColl = enemyRef[i]->pos[0] + 70 >= ki.kiTracker[kiRef][0]
			&& ki.kiTracker[kiRef][0] + 15 >= enemyRef[i]->pos[0];
		bool yColl = enemyRef[i]->pos[1] + 50 >= ki.kiTracker[kiRef][1]
			&& ki.kiTracker[kiRef][1] + 30 >= enemyRef[i]->pos[1];
		if (xColl && yColl) {
			kiFree(kiRef);
			detection(i);
			break;
		}
	}
}

void bossCollision(int kiRef)
{
	bool xColl = finBoss->pos[0] + 200 >= ki.kiTracker[kiRef][0]
		&& ki.kiTracker[kiRef][0] + 15 >= finBoss->pos[0];
	bool yColl = finBoss->pos[1] + 150 >= ki.kiTracker[kiRef][1]
		&& ki.kiTracker[kiRef][1] + 30 >= finBoss->pos[1];
	if (xColl && yColl) {
		kiFree(kiRef);
		//insert Boss update function here

		bossDetection();

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
			if (goku.currentPic > 0) {
				goku.moveS -= goku.currentPic;
				goku.currentPic--;
			}
			detection(i);
			healthCheck();
		}
	}
	for (int i = 0; i < MAX_BRACE; i++) {
		bool xColl = brace.bossTracker[i][0] + 40 >= goku.pos[0]
			&& goku.pos[0] + goku.width >= brace.bossTracker[i][0];
		bool yColl = brace.bossTracker[i][1] + 20 >= goku.pos[1]
			&& goku.pos[1] + goku.height >= brace.bossTracker[i][1];
		if (xColl && yColl) {
			goku.health--;
			if (goku.currentPic > 0) {
				goku.moveS -= goku.currentPic;
				goku.currentPic--;
			}
			braceFree(i);
			healthCheck();
			printf("Brace hit.\n");
		}
	}
	//Boss Collision
	bool xColl = finBoss->pos[0] + 70 >= goku.pos[0]
		&& goku.pos[0] + goku.width >= finBoss->pos[0];
	bool yColl = finBoss->pos[1] + 50 >= goku.pos[1]
		&& goku.pos[1] + goku.height >= finBoss->pos[1];
	if (xColl && yColl) {
		goku.health--;
		if (goku.currentPic > 0) {
			goku.moveS -= goku.currentPic;
			goku.currentPic--;
		}
		healthCheck();
	}
}

//Collision detection with powerups.
void gokuPower()
{
	for (int i = 0; i < plimiter; i++) {
		bool xColl = powRef[i]->pos[0] + 70 >= goku.pos[0]
			&& goku.pos[0] + goku.width >= powRef[i]->pos[0];
		bool yColl = powRef[i]->pos[1] + 50 >= goku.pos[1]
			&& goku.pos[1] + goku.height >= powRef[i]->pos[1];
		if (xColl && yColl) {
			goku.health++;
			if (goku.currentPic < 2 && goku.health > 3 + goku.currentPic) {
				goku.currentPic++;
				goku.moveS += goku.currentPic;
			}
			//insert powerup removal function here
			break;
		}
	}
	//Boss Collision
	bool xColl = finBoss->pos[0] + 70 >= goku.pos[0]
		&& goku.pos[0] + goku.width >= finBoss->pos[0];
	bool yColl = finBoss->pos[1] + 50 >= goku.pos[1]
		&& goku.pos[1] + goku.height >= finBoss->pos[1];
	if (xColl && yColl) {
		goku.health--;
		if (goku.currentPic > 0) {
			goku.moveS -= goku.currentPic;
			goku.currentPic--;
		}
		healthCheck();
	}
}

//Collision detection with powerups.
void gokuPower()
{
	for (int i = 0; i < plimiter; i++) {
		bool xColl = powRef[i]->pos[0] + 70 >= goku.pos[0]
			&& goku.pos[0] + goku.width >= powRef[i]->pos[0];
		bool yColl = powRef[i]->pos[1] + 50 >= goku.pos[1]
			&& goku.pos[1] + goku.height >= powRef[i]->pos[1];
		if (xColl && yColl) {
			goku.health++;
			if (goku.currentPic < 2 && goku.health > 3 + goku.currentPic) {
				goku.currentPic++;
				goku.moveS += goku.currentPic;
			}
			//insert powerup removal function here
			break;
		}
	}
}

void healthCheck()
{
	if (goku.health <= 0) {
		//printf("You've ran out of health.\n");
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

void braceHandler(int type)
{
	if (finBoss->pos[0] < g.xres) {
		for (int i = 0; i < MAX_BRACE; i++) {
			if (brace.bossTracker[i][0] == UNASSIGN &&
				brace.bossTracker[i][1] == UNASSIGN) {
				launchBrace(type);
			} else {
				if (type == 0) {
					braceMove(i);
				} else if (type == 1) {
					braceRender(i);
				}
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
