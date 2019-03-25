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

class Global {
	public:
		int xres, yres;
} glob;

class kiBlast {
	public:
		GLuint image;
		int kiTracker[10][2];
		int kiVel;
} ki;

void kiInit()
{
	ki.kiVel = .005;
	for (int i = 0; i < 10; i++) {
		ki.kiTracker[i][0] = 0;
		ki.kiTracker[i][1] = 0;
	}
}

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

int kiLimitCheck()
{
	for (int i = 0; i < 10; i++) {
		if (ki.kiTracker[i][0] == 0 && ki.kiTracker[i][1] == 0)
			return i;
	}
	return -1;
}

void launchKi(int x, int y)
{
	int kiNum = kiLimitCheck();
	if (kiNum != -1) {
		ki.kiTracker[kiNum][0] = x;
		ki.kiTracker[kiNum][1] = y;
	}
}

void kiFree(int kiID)
{
	ki.kiTracker[kiID][0] = 0;
	ki.kiTracker[kiID][1] = 0;
}

void kiMove(int kiID)
{
	if (ki.kiTracker[kiID][0] > glob.xres/2) {
		kiFree(kiID);
	} else {
		ki.kiTracker[kiID][0] += 10;
	}
}

void kiRender(int kiID)
{
	float cx = glob.xres/2.0;
	float cy = glob.yres/2.0;
	float h = 20.0;
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

void kiHandler(int type)
{
	for (int i = 0; i < 10; i++) {
		if (ki.kiTracker[i][0] != 0) {
			if (!type)
				kiMove(i);
			else if (type == 1)
				kiRender(i);
		}
	}
}
