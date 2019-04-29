//3350
//Program:  joshuaA.cpp
//Author:   Joshua Annis
//Date Last Modified: 4/24/19
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
/*
 * My file includes the background to the game and
 * my own personal picture in the credits.
 *
 */
extern Global g;
extern void powerReference(Powerups*);
extern void powerCollision();
Powerups powerups;
BlastPowerup blastPowerup;
//------------------Background----------------------------------
void setBackgroundNamek(int x, int y, GLuint textInt)
{
    glBindTexture(GL_TEXTURE_2D, textInt);
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(0, 35); //bottom left
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(0, y); //top left
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(x, y); //top right
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(x, 35);
    glEnd();
}
//--------------------------------------------------------------
void showJoshuaText(int x, int y)
{
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y-20;
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
    glVertex2i(x+525, y+250); //bottom left
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(x+525, y+500); //top left
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(x+675, y+500); //top right
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(x+675, y+250);
    glEnd();
}

void showJoshua(int x, int y, GLuint textInt)
{
    showJoshuaPic(0, 0, textInt);
    showJoshuaText(x+525, y+100);

}
//----------------------Power-ups----------------------------
void Powerups_init ()
{
    srand(time(NULL));
    powerups.pos[0] = g.xres + (rand() % 100);
    powerups.pos[1] = (rand() % g.yres);
    powerReference(&powerups);
}

void powerupsPhysics ()
{
    powerups.pos[0] -= 2;
    if (powerups.pos[0] < -50) {
	powerups.pos[0] = g.xres;
	powerups.pos[1] = (rand() % (g.yres - 100) + 1);
    }
    powerCollision();
}

void powerupsRender (GLuint image)
{
    glPushMatrix();
    glTranslated(powerups.pos[0], powerups.pos[1], powerups.pos[2]);
    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, image);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    float tx = 0, ty = 0;

    // Render powerups
    glBegin(GL_QUADS);
    glTexCoord2f(tx+1, ty+1);
    glVertex2i(0, 0);
    glTexCoord2f(tx+1, ty);
    glVertex2i(0, 75);
    glTexCoord2f(tx, ty);
    glVertex2i(100, 75);
    glTexCoord2f(tx, ty+1);
    glVertex2i(100, 0);
    glEnd();

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}
void blastPowerup_init()
{
    srand(time(NULL));
    blastPowerup.pos[0] = g.xres + (rand() % 100);
    blastPowerup.pos[1] = (rand() % g.yres);
    //	powerReference(&blastPowerup);
}
void blastPowerupPhysics()
{
    blastPowerup.pos[0] -=2;
    if(blastPowerup.pos[0] < -50) {
	blastPowerup.pos[0] = g.xres;
	blastPowerup.pos[1] = (rand() % g.yres);
    }
    //blastCollision();
}
void blastPowerupRender (GLuint image)
{
    glPushMatrix();
    glTranslated(blastPowerup.pos[0], blastPowerup.pos[1], blastPowerup.pos[2]);
    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, image);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    float tx = 0, ty = 0;

    // Render powerups
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
class NamekBackground {
    private: 
	float xc[2];
	float yc[2];
    public:
	NamekBackground();
	NamekBackground(float xc1, float xc2, int);
	NamekBackground(float yc1, float yc2, std::string);
	NamekBackground(float xc1, float xc2, float yc1, float yc2);
	void setX(float xc1, float xc2);
	void setY(float yc1, float yc2);
	float * getX();
	float * getY();
} background;


NamekBackground::NamekBackground()
{
    background.xc[0] = 0.0;
    background.xc[1] = 0.25;
    background.yc[0] = 0.0;
    background.yc[1] = 1.0;

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
	float yc2
	)
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

float * NamekBackground::getX()
{
    return xc;
}

float * NamekBackground::getY()
{
    return yc;
}

