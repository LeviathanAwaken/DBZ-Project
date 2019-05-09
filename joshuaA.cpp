//3350
//Program:  joshuaA.cpp
//Author:   Joshua Annis
//Date Last Modified: 5/9/19
#include <GL/glx.h>
#include "fonts.h"
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#include "Global.h"
#include "Powerups.h"
#include "BlastPowerup.h"
#include <string>
#include "Timers.h"
/*
 * My file includes the background to the game and
 * my own personal picture in the credits.
 *
 */
extern Global g;
extern Timers timers;
extern void powerReference(Powerups *);
extern void blastPowerReference(float *, float *);
extern void powerCollision();
extern void blastCollision();
extern void acceptGameState(int);
extern int selectedOption;
extern int gameState;
float xticks = 0.0;
int speed_Randomizer(void);
int freq_Randomizer(void);
int amp_Randomizer(void);

Powerups powerups;
BlastPowerup blastPowerup;

class NamekBackground
{
    public:
	float xc[2];
	float yc[2];

    public:
	NamekBackground();
	NamekBackground(float xc1, float xc2, int);
	NamekBackground(float yc1, float yc2, std::string);
	NamekBackground(float xc1, float xc2, float yc1, float yc2);
	void setX(float xc1, float xc2);
	void setY(float yc1, float yc2);
	float *getX();
	float *getY();
} background;

NamekBackground::NamekBackground()
{
    background.xc[0] = 0.0;
    background.xc[1] = 1.0;
    background.yc[0] = 0.0;
    background.yc[1] = 1.0;
}

//------------------Background----------------------------------
void setBackgroundNamek(int x, int y, GLuint textInt)
{
    // glBindTexture(GL_TEXTURE_2D, textInt);
    // glColor4f(1, 1, 1, 1);
    // glBegin(GL_QUADS);

    // glTexCoord2f(0.0f, 1.0f);
    // glVertex2i(0, 35); //bottom left

    // glTexCoord2f(0.0f, 0.0f);
    // glVertex2i(0, y); //top left

    // glTexCoord2f(1.0f, 0.0f);
    // glVertex2i(x, y); //top right

    // glTexCoord2f(1.0f, 1.0f);
    // glVertex2i(x, 35);

    // glEnd();

    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, textInt);
    glBegin(GL_QUADS);

    glTexCoord2f(background.xc[0], background.yc[1]);
    glVertex2i(0, 35);

    glTexCoord2f(background.xc[0], background.yc[0]);
    glVertex2i(0, y);

    glTexCoord2f(background.xc[1], background.yc[0]);
    glVertex2i(x, y);

    glTexCoord2f(background.xc[1], background.yc[1]);
    glVertex2i(x, 35);

    glEnd();
}
//--------------------------------------------------------------
void showJoshuaText(int x, int y)
{
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y - 20;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 22, c, "Joshua Annis");
}

void showJoshuaPic(int x, int y, GLuint textInt)
{
    glBindTexture(GL_TEXTURE_2D, textInt);
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(x + 525, y + 250); //bottom left
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(x + 525, y + 500); //top left
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(x + 675, y + 500); //top right
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(x + 675, y + 250);
    glEnd();
}

void showJoshua(int x, int y, GLuint textInt)
{
    showJoshuaPic(0, 0, textInt);
    showJoshuaText(x + 525, y + 100);
}
//----------------------Power-ups----------------------------
void Powerups_init()
{
    srand(time(NULL));
    powerups.pos[0] = g.xres + (rand() % 100);
    powerups.pos[1] = (rand() % g.yres);
    powerReference(&powerups);
}

void powerupsPhysics()
{
    powerups.pos[0] -= 2;
    if (powerups.pos[0] < -50)
    {
	powerups.pos[0] = g.xres;
	powerups.pos[1] = (rand() % (g.yres - 100) + 1);
    }
    powerCollision();
}

void powerupsRender(GLuint image)
{
    glPushMatrix();
    glTranslatef(powerups.pos[0], powerups.pos[1], 0);
    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, image);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255, 255, 255, 255);

    float tx = 0, ty = 0;

    // Render powerups
    glBegin(GL_QUADS);
    glTexCoord2f(tx + 1, ty + 1);
    glVertex2i(0, 0);
    glTexCoord2f(tx + 1, ty);
    glVertex2i(0, 75);
    glTexCoord2f(tx, ty);
    glVertex2i(100, 75);
    glTexCoord2f(tx, ty + 1);
    glVertex2i(100, 0);
    glEnd();

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}
void blastPowerup_init()
{
    //srand(time(NULL));
    blastPowerup.pos[0] = g.xres + (rand() % 100);
    blastPowerup.pos[1] = (rand() % g.yres);
    //blastPowerReference(&blastPowerup);
}
void blastPowerupPhysics()
{
    blastPowerup.pos[0] -= 2;
    if (blastPowerup.pos[0] < -50)
    {
	blastPowerup.pos[0] = g.xres;
	blastPowerup.pos[1] = (rand() % (g.yres - 100) + 1);
    }
    //blastCollision();
}
void blastPowerupRender(GLuint image)
{
    glPushMatrix();
    glTranslatef(blastPowerup.pos[0], blastPowerup.pos[1], 0);
    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, image);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255, 255, 255, 255);

    float tx = 0, ty = 0;

    // Render powerups
    glBegin(GL_QUADS);
    glTexCoord2f(tx + 1, ty + 1);
    glVertex2i(0, 0);
    glTexCoord2f(tx + 1, ty);
    glVertex2i(0, 50);
    glTexCoord2f(tx, ty);
    glVertex2i(70, 50);
    glTexCoord2f(tx, ty + 1);
    glVertex2i(70, 0);
    glEnd();

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

NamekBackground::NamekBackground(float xc1, float xc2, int a)
{
    a = 5;
    xc[0] = a;
    xc[0] = xc1;
    xc[1] = xc2;
}

NamekBackground::NamekBackground(float yc1, float yc2, std::string a)
{
    a = "0";
    yc[0] = yc1;
    yc[1] = yc2;
}

NamekBackground::NamekBackground(
	float xc1,
	float xc2,
	float yc1,
	float yc2)
{
    xc[0] = xc1;
    xc[1] = xc2;
    yc[0] = yc1;
    yc[1] = yc2;
}

void NamekBackground::setX(float xc1, float xc2)
{
    xc[0] = xc1;
    xc[1] = xc2;
}

void NamekBackground::setY(float yc1, float yc2)
{
    yc[0] = yc1;
    yc[1] = yc2;
}

float *NamekBackground::getX()
{
    return xc;
}

float *NamekBackground::getY()
{
    return yc;
}

void namekPhysics()
{
    background.xc[0] += 0.001;
    background.xc[1] += 0.001;
}
//--------------------Dragon Balls--------------------------------------------------
class Dragonball{
    public:
	GLuint pics[7];
	int currentPic;
	float pos[2];
	int xSpeed = 0;
	int waveamp = 0;
	int wavefreq = 0;
	int wavepos = 0;
} dball;

void dballRender();

void dballInit(GLuint dball1, GLuint dball2, GLuint dball3,
	GLuint dball4, GLuint dball5, GLuint dball6, GLuint dball7)
{
    dball.pics[0] = dball1;
    dball.pics[1] = dball2;
    dball.pics[2] = dball3;
    dball.pics[3] = dball4;
    dball.pics[4] = dball5;
    dball.pics[5] = dball6;
    dball.pics[6] = dball7;
    dball.currentPic = 0;
    dball.pos[0] = g.xres + 500;
    dball.pos[1] = g.yres/2;
    blastPowerReference(&dball.pos[0], &dball.pos[1]);
}


void dballPhysics()
{    
    xticks += 2.7;
    dball.pos[0] -= (dball.xSpeed + 4);
    dball.pos[1] = (50 * sin(xticks/150) + g.yres/2);

    if (dball.pos[0] < -50){
	dball.pos[0] = g.xres + 500;
	dball.xSpeed = speed_Randomizer();
	dball.pos[1] = (rand() % (g.yres));

    }

    blastCollision();

}

void dballScore()
{
    int range = g.score % 2000;

    if(abs(2000 - range) <= 100 && dball.pos[0] > -50)
    {
	range = 0;
	dballRender();
	
    }
}
void dballWinCondition()
{
    if (dball.currentPic > 6) {
	gameState = WIN;
    }
}

void dballCollected()
{
    dball.currentPic++;

    dball.pos[0] = g.xres + 1500;
    dball.pos[1] = (rand() % (g.yres - 100) + 1);
    dballScore();
    dballWinCondition();
}   

void dballRender()
{   
    glPushMatrix();
    glTranslatef(dball.pos[0], dball.pos[1], 0);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, dball.pics[dball.currentPic]);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    float tx = 0.0;
    float ty = 0.0;

    glBegin(GL_QUADS);
    glTexCoord2f(tx+1, ty+1);
    glVertex2i(0, 0);
    glTexCoord2f(tx+1, ty);
    glVertex2i(0, 50);
    glTexCoord2f(tx, ty);
    glVertex2i(70, 50);
    glTexCoord2f(tx, ty+1);
    glVertex2i(70, 0);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

void renderWinScreen(int x, int y, GLuint textint) {
	glClearColor(0.1, 0.1, 0.1, 1.0);
  	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, textint);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);

	float tx = 0, ty = 0;
	glBegin(GL_QUADS);
	
	glTexCoord2f(tx, ty + 1);
	glVertex2i(0,35);

	glTexCoord2f(tx,ty);
	glVertex2i(0, y);

	glTexCoord2f(tx+1, ty);
	glVertex2i(x, y);

	glTexCoord2f(tx+1, ty+1);
	glVertex2i(x, 35);
	glEnd();

	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);

	Rect r;
	r.bot = g.yres/3;
	r.left = g.xres/2;
	r.center = 1;

	switch (selectedOption) {
		case 0:
			ggprint8b(&r, 16, 0x123fff, "NEW GAME");
			ggprint8b(&r, 16, 0xffffff, "CREDITS");
			ggprint8b(&r, 16, 0xffffff, "TOGGLE CONTROLS");
			ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
			break;
		case 1: 
			ggprint8b(&r, 16, 0xffffff, "NEW GAME");
			ggprint8b(&r, 16, 0x123fff, "CREDITS");
			ggprint8b(&r, 16, 0xffffff, "TOGGLE CONTROLS");
			ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
			break;
		case 2:
			ggprint8b(&r, 16, 0xffffff, "NEW GAME");
			ggprint8b(&r, 16, 0xffffff, "CREDITS");
			ggprint8b(&r, 16, 0x123fff, "TOGGLE CONTROLS");
			ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
			break;
		case 3:
			ggprint8b(&r, 16, 0xffffff, "NEW GAME");
			ggprint8b(&r, 16, 0xffffff, "CREDITS");
			ggprint8b(&r, 16, 0xffffff, "TOGGLE CONTROLS");
			ggprint8b(&r, 16, 0x123fff, "EXIT GAME");
			break;
		default:
			break;
	}
}

void checkKeysWin()
{
    double menuSelectionDelay = 0.15;
    if (g.keys[XK_Up]) {
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
		&timers.timeCurrent);
	if (timeSpan > menuSelectionDelay) {
	    selectedOption = (((selectedOption-1)+4)%4);
	    timers.recordTime(&timers.menuSelectionTime);

	}
    }
    if (g.keys[XK_Down]) {
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
		&timers.timeCurrent);
	if (timeSpan > menuSelectionDelay) {
	    selectedOption = (((selectedOption+1)+4)%4);
	    timers.recordTime(&timers.menuSelectionTime);

	}
    }
    if (g.keys[XK_Return]) {
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
		&timers.timeCurrent);
	if (timeSpan > menuSelectionDelay) {
	    acceptGameState(selectedOption);
	    timers.recordTime(&timers.menuSelectionTime);
	}
    }
}
