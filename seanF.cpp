//3350
//Program:  seanF.cpp
//Author:   Sean Fontes
//Date:     2-14-19

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
