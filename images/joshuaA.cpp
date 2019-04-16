//3350
//Program:  joshuaA.cpp
//Author:   Joshua Annis
//Date Last Modified:     3/3/19
#include <GL/glx.h>
#include "fonts.h"
/* 
 * My file includes the background to the game and
 * my own personal picture in the credits.
 *
 */

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

//class 
