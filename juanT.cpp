#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"

void showJuanPic(int x, int y, GLuint textid)
{
	glColor3ub(255,255,255);
	//static float angle = 0.0f;
	static int wid = 100;
	//wid += sin(angle) * 10;
	float fx = (float)x;
	float fy = (float)y;
	/*float a = 0;
	a += sin(angle) * 10.0f;
	fx += a;
	angle += 0.2f;*/
	glPushMatrix();
	glTranslatef(fx,fy,0);
	//glRotatef(a,0,0,1.0);
	glBindTexture(GL_TEXTURE_2D, textid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);glVertex2i(-wid-50,-wid);
		glTexCoord2f(0.0f,0.0f);glVertex2i(-wid-50,wid);
		glTexCoord2f(1.0f,0.0f);glVertex2i(wid,wid);
		glTexCoord2f(1.0f,1.0f);glVertex2i(wid,-wid);
	glEnd();
	glPopMatrix();
}

void showJuanText(int x, int y)
{
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y-20;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 22, c, "Juan Trigueros");
}

void showJuan(int x, int y, GLuint textInt)
{
	showJuanPic(x+200,y-200,textInt);
	showJuanText(x+125,y-300);
}
