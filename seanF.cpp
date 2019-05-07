//3350
//Program:  		seanF.cpp
//Author:   		Sean Fontes
//Date:     		2-14-19
//Last Modified:	5-7-19

/*
 * Description:
 * File controls the bulk of the collision detection for the game. It
 * also controls the character, including movement and blasts, as well
 * as the projectiles for the boss.
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
#include "Image.h"
#include "lawrenceM.h"
#include "BlastPowerup.h"
#ifdef SOUND
	#include </usr/include/AL/alut.h>
	#include <unistd.h>
#endif

const int MAX_KI = 20;
const int MAX_BRACE = 2;
const int UNASSIGN = -5000;
const int MAX_ENEM = 15;

Enemy *enemyRef[MAX_ENEM];
Boss *finBoss;
Powerups *powRef[2];
BlastPowerup *blastPower;
int elimiter;
int plimiter;
extern int gameState;
extern Image img[];
extern healthSSD healthBar;
extern healthSSD dBallCounter;
//Prototypes and extern function declarations
void kiCollision(int);
void kiHandler(int);
void braceHandler(int);
void bossHitCollision(int);
void braceCollision(int);
void gokuRender();
void healthCheck();
bool gokuBounds(int);
extern void detection(int, bool);
extern void bossDetection();
extern int score_update(int);
extern void energyRender();

//Class encompassing the main character's position and other attributes.
class Protag {
	public:
		GLuint pics[6];
		int currentPic;
		float pos[2];
		//float vel[2];
		int height;
		int width;
		int health;
		int dballs;
		float moveS;
} goku;

class Energy {
	public:
		float *centerX;
		float *centerY;
		float height;
		float width;
		int spriteSheetIndex;
		int frame;
		void draw();
} outline;

//Class to track x and y positions of the blasts being sent by the character.
class kiBlast {
	public:
		GLuint image;
		float kiTracker[MAX_KI][2];
		float kiVel;
		float height;
		float width;
		int spriteSheetIndex;
		int frame[MAX_KI];
		void draw(int);
} ki;

//Class to track x and y positions of boss's blasts
class bossProjectile {
	public:
		GLuint image;
		int bossTracker[MAX_BRACE][2];
		int bossXVel;
		int bossYVel[MAX_BRACE];
} brace;

//Establishes a pointer to enemies being handled in Drake's file.
void enemyReference(Enemy* enem)
{
	enemyRef[elimiter] = enem;
	elimiter++;
}

//Establishes a pointer to the boss being handled in Drake's file.
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

//Establishes a pointer to dragon balls being handled in Josh's file.
void blastPowerReference(BlastPowerup* blast)
{
	blastPower = blast;
}

//Initializes the kiClass for use.
void kiInit()
{
	ki.kiVel = 7;
	for (int i = 0; i < MAX_KI; i++) {
		ki.kiTracker[i][0] = UNASSIGN;
		ki.kiTracker[i][1] = UNASSIGN;
		ki.frame[i] = 0;
	}
	ki.height = img[7].height / 1.5;
	ki.width = img[7].width / (9 * 1.5);
	ki.spriteSheetIndex = 7;
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
	goku.dballs = 0;
	goku.currentPic = 0;
	healthBar.updateHealthCounter(goku.health);
}

void outlineInit()
{
	outline.centerX = &goku.pos[0];
	outline.centerY = &goku.pos[1];
	outline.height = img[24].height / 1.5;
	outline.width = img[24].width / (4 * 1.5);
	outline.spriteSheetIndex = 24;
	outline.frame = 0;
}

//Initializes the projectiles for the boss.
void braceInit()
{
	brace.bossXVel = -6;
	for (int i = 0; i < MAX_BRACE; i++) {
		brace.bossTracker[i][0] = UNASSIGN;
		brace.bossTracker[i][1] = UNASSIGN;
	}
	srand(time(NULL));
}

//Generalized initializer for the file, called in the main file.
void sInit(GLuint gok, GLuint gok2, GLuint gok3, GLuint gok4, GLuint gok5,
	GLuint gok6)
{
	elimiter = 0;
	plimiter = 0;
	goku.pics[0] = gok;
	goku.pics[1] = gok2;
	goku.pics[2] = gok3;
	goku.pics[3] = gok4;
	goku.pics[4] = gok5;
	goku.pics[5] = gok6;
	gokuInit();
	kiInit();
	braceInit();
	outlineInit();
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

//Goku's movement logic to be called from the main file.
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
}

//Logic handling for the bounds on character's movement.
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

//Generalized render function.
void sRender()
{
	if (goku.currentPic > 0) {
		energyRender();
	}
	gokuRender();
	kiHandler(1);
	braceHandler(1);
}

//Main character render handler.
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
	for (int i = 0; i < MAX_KI - (10 - (2 * goku.currentPic)); i++) {
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
		ki.kiTracker[kiNum][0] = goku.pos[0] + 20;
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
	bossHitCollision(kiID);
}

//Handles graphics for the kiBlasts.
void kiBlast::draw(int kiID)
{
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, g.kiTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255, 255, 255, 255);

	float ssWidth = (float) 1.0 / img[spriteSheetIndex].columns;
	float ssHeight = (float) 1.0 / img[spriteSheetIndex].rows;

	int ix = this->frame[kiID] % img[spriteSheetIndex].columns;
	int iy = 0;

	for (int i = 1; i < 9; i++) {
		if (frame[kiID] >= img[spriteSheetIndex].columns * i) {
			iy = i;
		}
	}

	float textureX = (float) ix / img[spriteSheetIndex].columns;
	float textureY = (float) iy / img[spriteSheetIndex].rows;

	glBegin(GL_QUADS);
	glTexCoord2f(textureX, textureY+ssHeight);
	glVertex2i((kiTracker[kiID][0] + goku.width + 2)-width,
		(kiTracker[kiID][1] + goku.height/2)-height);

	glTexCoord2f(textureX, textureY);
	glVertex2i((kiTracker[kiID][0] + goku.width + 2)-width,
		(kiTracker[kiID][1] + goku.height/2)+height);

	glTexCoord2f(textureX+ssWidth, textureY);
	glVertex2i((kiTracker[kiID][0] + goku.width + 2)+width,
		(kiTracker[kiID][1] + goku.height/2)+height);

	glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
	glVertex2i((kiTracker[kiID][0] + goku.width + 2)+width,
		(kiTracker[kiID][1] + goku.height/2)-height);
	glEnd();

	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);

	frame[kiID]++;
	if (frame[kiID] >= img[spriteSheetIndex].columns * 8) {
		frame[kiID] = 0;
	}
}

//Creates a kiBlast, if it's not at max.
void launchBrace(int id)
{
	brace.bossTracker[id][0] = finBoss->pos[0] - 20;
	brace.bossTracker[id][1] = finBoss->pos[1];
	brace.bossYVel[id] = (rand() % 5 + 1) * ((rand() % 2 == 1) ? 1 : -1);
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
		brace.bossTracker[braceID][1] + 40 > g.yres ||
		brace.bossTracker[braceID][1] < 0) {
		braceFree(braceID);
	} else {
		brace.bossTracker[braceID][0] -= 6;
		brace.bossTracker[braceID][1] += brace.bossYVel[braceID];
		braceCollision(braceID);
	}
}

//Renders boss's projectiles.
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
			&& ki.kiTracker[kiRef][0] + ki.width >= enemyRef[i]->pos[0];
		bool yColl = enemyRef[i]->pos[1] + 50 >= ki.kiTracker[kiRef][1]
			&& ki.kiTracker[kiRef][1] + ki.height >= enemyRef[i]->pos[1];
		if (xColl && yColl) {
			kiFree(kiRef);
			detection(i, false);
			break;
		}
	}
}

//Boss hit by a ki blast collission.
void bossHitCollision(int kiRef)
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

//Goku hit by curly brace collision.
void braceCollision(int braceID)
{
	bool xColl = brace.bossTracker[braceID][0] + 40 >= goku.pos[0]
		&& goku.pos[0] + goku.width >= brace.bossTracker[braceID][0];
	bool yColl = brace.bossTracker[braceID][1] + 20 >= goku.pos[1]
		&& goku.pos[1] + goku.height >= brace.bossTracker[braceID][1];
	if (xColl && yColl) {
		goku.health--;
		healthBar.updateHealthCounter(goku.health);
		if (goku.currentPic > 0 && goku.health < 8) {
			goku.moveS -= 2;
			goku.currentPic--;
		}
		braceFree(braceID);
		healthCheck();
	}
}

//Goku hit by the boss.
void bossCollision()
{
	bool xColl = finBoss->pos[0] + 70 >= goku.pos[0]
		&& goku.pos[0] + goku.width >= finBoss->pos[0];
	bool yColl = finBoss->pos[1] + 50 >= goku.pos[1]
		&& goku.pos[1] + goku.height >= finBoss->pos[1];
	if (xColl && yColl) {
		goku.health--;
		healthBar.updateHealthCounter(goku.health);
		if (goku.currentPic > 0 && goku.health < 8) {
			goku.moveS -= 2;
			goku.currentPic--;
		}
		healthCheck();
	}
}

//Collision checking for the main character and the enemies.
void saibaCollision()
{
	for (int i = 0; i < MAX_ENEM; i++) {
		bool xColl = enemyRef[i]->pos[0] + 70 >= goku.pos[0]
			&& goku.pos[0] + goku.width >= enemyRef[i]->pos[0];
		bool yColl = enemyRef[i]->pos[1] + 50 >= goku.pos[1]
			&& goku.pos[1] + goku.height >= enemyRef[i]->pos[1];
		if (xColl && yColl) {
			goku.health--;
			healthBar.updateHealthCounter(goku.health);
			if (goku.currentPic > 0 && goku.health < 8) {
				goku.moveS -= 2;
				goku.currentPic--;
			}
			detection(i, true);
			healthCheck();
		}
	}
}

//Collision detection with powerups.
void powerCollision()
{
	for (int i = 0; i < plimiter; i++) {
		bool xColl = powRef[i]->pos[0] + 70 >= goku.pos[0]
			&& goku.pos[0] + goku.width >= powRef[i]->pos[0];
		bool yColl = powRef[i]->pos[1] + 50 >= goku.pos[1]
			&& goku.pos[1] + goku.height >= powRef[i]->pos[1];
		if (xColl && yColl) {
			goku.health++;
			healthBar.updateHealthCounter(goku.health);
			if (goku.currentPic < 5 && goku.health > 3) {
				goku.currentPic++;
				goku.moveS += 2;
			}
			powRef[i]->pos[0] = g.xres;
			powRef[i]->pos[1] = (rand() % (g.yres - 100) + 1);
			score_update(25);
			break;
		}
	}
}

void blastCollision()
{
	bool xColl = blastPower->pos[0] + 70 >= goku.pos[0]
		&& goku.pos[0] + goku.width >= blastPower->pos[0];
	bool yColl = blastPower->pos[1] + 50 >= goku.pos[1]
		&& goku.pos[1] + goku.height >= blastPower->pos[1];
	if (xColl && yColl) {
		goku.dballs++;
		dBallCounter.updateDBallCounter(goku.dballs);
		blastPower->pos[0] = g.xres;
		blastPower->pos[1] = (rand() % (g.yres - 100) + 1);
	}
}

//Basic health check, calls death screen.
void healthCheck()
{
	if (goku.health <= 0) {
		gameState = DEATH;
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
				ki.draw(i);
			}
		}
	}
}

//Handler function for movement and rendering.
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

//Prints character's health on screen.
void gokuHealth(int x, int y)
{
	Rect r;
	unsigned int c = 0x00ffff44;
	r.bot = y+20;
	r.left = x+300;
	r.center = 0;
	ggprint16(&r, 16, c,"Goku Health: %d", goku.health);
}

void energyRender()
{
	outline.draw();
}

void Energy::draw()
{
	GLuint text;
	if (goku.currentPic > 3 && goku.currentPic < 5)
		text = g.redTexture;
	else if (goku.currentPic == 5)
		text = g.blueTexture;
	else if (goku.currentPic == 2)
		text = g.ss3Outline;
	else if (goku.currentPic == 3)
		text = g.ss4Outline;
	else
		text = g.outlineTexture;
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, text);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255, 255, 255, 255);

	float ssWidth = (float) 1.0 / img[spriteSheetIndex].columns;
	float ssHeight = (float) 1.0 / img[spriteSheetIndex].rows;

	int ix = this->frame % img[spriteSheetIndex].columns;
	int iy = 0;

	for (int i = 1; i < 9; i++) {
		if (frame >= img[spriteSheetIndex].columns * i) {
			iy = i;
		}
	}

	float textureX = (float) ix / img[spriteSheetIndex].columns;
	float textureY = (float) iy / img[spriteSheetIndex].rows;

	glBegin(GL_QUADS);
	glTexCoord2f(textureX, textureY+ssHeight);
	glVertex2i((*centerX + (goku.width/2))-width,
		(*centerY + 30 + (goku.height/2))-height);

	glTexCoord2f(textureX, textureY);
	glVertex2i((*centerX + (goku.width/2))-width,
		(*centerY + 30 + (goku.height/2))+height);

	glTexCoord2f(textureX+ssWidth, textureY);
	glVertex2i((*centerX + (goku.width/2))+width,
		(*centerY + 30 + (goku.height/2))+height);

	glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
	glVertex2i((*centerX + (goku.width/2))+width,
		(*centerY + 30 + (goku.height/2))-height);
	glEnd();

	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);

	frame++;
	if (frame >= img[spriteSheetIndex].columns * 8) {
		frame = 0;
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
		albuffer = alutCreateBufferFromFile("./sounds/SuperSaiyan.mp3");

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
