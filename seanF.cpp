//3350
//Program:  seanF.cpp
//Author:   Sean Fontes
//Date:     2-14-19

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
    /*glColor3ub(255, 255, 255);
    static float angle = 0.0f;
    static int wid = 30;
    wid += sin(angle) * 10;
    float sx = (float)x;
    float sy = (float)y-40;
    float a = 0;
    a += sin(angle) * 10.0f;
    sx += a;
    angle += 0.2f;
    glPushMatrix();
    glTranslatef(sx, sy, 0);
    glRotatef(a, 0, 0, 1.0);*/
    
    glBindTexture(GL_TEXTURE_2D, textInt);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, y/2);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(x, y/2);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(x, 0);
    glEnd();
    //glPopMatrix();
}

void showSean(int x, int y, GLuint textInt)
{
    showPic(x+200, y, textInt);
    showText(x, y);
}
