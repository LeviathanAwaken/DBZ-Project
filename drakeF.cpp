//3350
//Program:  drakeF.cpp
//Author:   Drake Floyd
//Date:     2/15/19



#include <GL/glx.h>
#include "fonts.h"

void showdrakeText(int x, int y)
{
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y-20;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, c, "Drake Floyd");
}

void showdrakePic(int x, int y, GLuint textInt)
{
    glBindTexture(GL_TEXTURE_2D, textInt);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(x+325, y+150); //bottom left
        glTexCoord2f(0.0f, 0.0f); glVertex2i(x+325, y+400); //top left
        glTexCoord2f(1.0f, 0.0f); glVertex2i(x+475, y+400); //top right
        glTexCoord2f(1.0f, 1.0f); glVertex2i(x+475, y+150);  
    glEnd();
}

void showDrake(int x, int y, GLuint textInt)
{
    showdrakePic(0, 0, textInt);
    showdrakeText(x+300, y+250);

}
