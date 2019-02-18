//3350
//Program:  joshuaA.cpp
//Author:   Joshua Annis
//Date:     2/15/19

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

void showJoshuaText(int x, int y)
{
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y-20;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, c, "Joshua Annis");
}

void showJoshuaPic(int x, int y, GLuint textInt)
{
    glBindTexture(GL_TEXTURE_2D, textInt);
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
