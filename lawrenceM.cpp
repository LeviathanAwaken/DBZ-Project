//3350
// Lawrence Marquez

<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
=======
>>>>>>> origin/master
#include <GL/glx.h>
#include "fonts.h"


<<<<<<< HEAD
void showText(int x, int y)
=======
void showLawrenceText(int x, int y)
>>>>>>> origin/master
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
<<<<<<< HEAD
    
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
=======
    glBindTexture(GL_TEXTURE_2D, textid);
    glColor4f(1, 1, 1, 1); 
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(x+500,y+50);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(x+500,y+200);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(x+650,y+200);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(x+650,y+50);
    glEnd();
}
void showLawrence(int x, int y,  GLuint textint)
{
    showLawrencePicture(0,0,textint);
    showLawrenceText(x+500, y-400);
>>>>>>> origin/master
}

