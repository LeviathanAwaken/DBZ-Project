//3350
// Lawrence Marquez

#include <GL/glx.h>
#include "fonts.h"


void showLawrenceText(int x, int y)
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
}