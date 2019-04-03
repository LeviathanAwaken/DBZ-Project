//3350
//Program:  joshuaA.cpp
//Author:   Joshua Annis
//Date Last Modified:     3/3/19
#include <GL/glx.h>
#include "fonts.h"
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#include "Global.h"
/*#include "Powerups.h"*/
/* 
 * My file includes the background to the game and
 * my own personal picture in the credits.
 *
 */
extern Global g;
//Powerups powerups[3];
//void pattern_1(Powerups&);
//------------------Background----------------------------------
void setBackgroundNamek(int x, int y, GLuint textInt)
{
    glBindTexture(GL_TEXTURE_2D, textInt);
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS); 
    glTexCoord2f(0.0f, 1.0f); glVertex2i(x+ 0, y+0); //bottom left
    glTexCoord2f(0.0f, 0.0f); glVertex2i(x+ 0, y+540); //top left
    glTexCoord2f(1.0f, 0.0f); glVertex2i(x+ 960, y+540); //top right
    glTexCoord2f(1.0f, 1.0f); glVertex2i(x+ 960, y+0);  
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
    glTexCoord2f(0.0f, 1.0f); glVertex2i(x+525, y+250); //bottom left
    glTexCoord2f(0.0f, 0.0f); glVertex2i(x+525, y+500); //top left
    glTexCoord2f(1.0f, 0.0f); glVertex2i(x+675, y+500); //top right
    glTexCoord2f(1.0f, 1.0f); glVertex2i(x+675, y+250);  
    glEnd();
}

void showJoshua(int x, int y, GLuint textInt)
{
    showJoshuaPic(0, 0, textInt);
    showJoshuaText(x+525, y+100);

}
//----------------------Power-ups----------------------------
/*void Powerups_init ()
{
    srand(time(NULL));
    for (int i = 0; i < 3; i++) {
	powerups[i].wavepos = (rand() % g.yres);
	int choice = (rand() % 4 + 1);
	[i].pattern = choice;
	powerups[i].pos[0] = g.xres + (rand() % 100);
	powerups[i].pos[1] = (rand() % g.yres); 
    }
}

void powerupsPhysics ()
{
    for (int i = 0; i < 3; i++) {
	if(enemy[i].pattern == 1)
	    pattern_1(enemy[i]);
    }
}

void powerupsRender (GLuint image)
{

    for (int i = 0; i < 3; i++) {
	glPushMatrix();
	glTranslated(powerups[i].pos[0], powerups[i].pos[1], powerups[i].pos[2]);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, image);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);


	float tx = 0, ty = 0;

	// Render powerups
	glBegin(GL_QUADS);
	glTexCoord2f(tx+1, ty+1); glVertex2i(0, 0);
	glTexCoord2f(tx+1, ty);   glVertex2i(0, 50);
	glTexCoord2f(tx, ty);     glVertex2i(70, 50);
	glTexCoord2f(tx, ty+1);   glVertex2i(70, 0);
	glEnd();



	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
    }

}
*/
