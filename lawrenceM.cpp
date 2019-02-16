//3350
// Lawrence Marquez

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
    ggprint8b(&r, 16, c, "lawrence marquez");
}

void showLawrencePicture (int x, int y, GLuint textid)
{
    
    glBindTexture(GL_TEXTURE_2D,glob.lawrenceTexture);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(x+525,y+250);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(x+525,y+500);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(x+675,y+500);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(x+675,y+500);

    glEnd();
  

}
void showLawrence(int x, int y,  GLuint textInt);
{
    showLawrencePicture(0,0,textint);
    showText(x+525, y+100);
}

