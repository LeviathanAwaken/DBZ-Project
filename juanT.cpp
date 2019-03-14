// Created By: Juan Trigueros
// Date: 02-14-19
//
//Shows credit pic, shows temporary start/pause screens and score keeper

#include <GL/glx.h>
#include "fonts.h"

void showJuanPic(int x, int y, GLuint textid)
{
    glBindTexture(GL_TEXTURE_2D, textid);
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,1.0f);glVertex2i(x+450,y+150);
        glTexCoord2f(0.0f,0.0f);glVertex2i(x+450,y+300);
        glTexCoord2f(1.0f,0.0f);glVertex2i(x+675,y+300);
        glTexCoord2f(1.0f,1.0f);glVertex2i(x+675,y+150);
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

void showScore(int x, int y, int z)
{
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y-20;
    r.left = x;
    r.center = 0;
    ggprint16(&r, 22, c,"Score: %d", z);
}

void showJuan(int x, int y, GLuint textInt)
{
    showJuanPic(x-400,y-950,textInt);
    showJuanText(x+125,y-800);
}

void showStart(int x, int y)
{
    Rect r;
    unsigned int c = 0x7c0095;
    r.bot = y-20;
    r.left = x;
    r.center = 0;
    ggprint16(&r, 16, c,"press z to start");
}

void showPause(int x, int y)
{
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y-20;
    r.left = x;
    r.center = 0;
    ggprint16(&r, 22, c,"paused");
}
