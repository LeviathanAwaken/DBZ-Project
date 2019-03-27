//3350
//Program:  drakeF.cpp
//Author:   Drake Floyd
//Date:     3/25/19


#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <vector>
#include "Global.h"
#include "Enemy.h"

extern Global g;
Enemy enemy[3];
float nticks = 0.0;
void Enemy_init();
void pattern_1(Enemy&);
void pattern_2(Enemy&);
void pattern_3(Enemy&);
void pattern_4(Enemy&);



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
        glTexCoord2f(1.0f, 1.0f); glVertex2i(x+475, y+150); //bottom right
    glEnd();
}

void showDrake(int x, int y, GLuint textInt)
{
    showdrakePic(0, 0, textInt);
    showdrakeText(x+300, y+250);

}

//-----------------------------movement for enemy work in progress-----------------------------
void Enemy_init ()
{   
    srand(time(NULL));
    for (int i = 0; i < 3; i++) {
        enemy[i].wavepos = (rand() % g.yres);
        int choice = (rand() % 4 + 1);
        enemy[i].pattern = choice;
        enemy[i].pos[0] = g.xres + (rand() % 100);
        enemy[i].pos[1] = (rand() % g.yres);
    }
}

void saibaPhysics () 
{
    

    for (int i = 0; i < 3; i++) {
        if(enemy[i].pattern == 1)
            pattern_1(enemy[i]);
        if(enemy[i].pattern == 2)
            pattern_2(enemy[i]);
        if(enemy[i].pattern == 3)
            pattern_3(enemy[i]);
        if(enemy[i].pattern == 4)
            pattern_4(enemy[i]);
    }
  
}

void saibaRender (GLuint image) 
{
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslated(enemy[i].pos[0], enemy[i].pos[1], enemy[i].pos[2]);
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, image);
        //
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);


            float tx = 0, ty = 0;

            // Render saibamen
            glBegin(GL_QUADS);
            glTexCoord2f(tx+1, ty+1); glVertex2i(0, 0);
            glTexCoord2f(tx+1, ty);   glVertex2i(0, 50);
            glTexCoord2f(tx, ty);     glVertex2i(70, 50);
            glTexCoord2f(tx, ty+1);   glVertex2i(70, 0);
            glEnd();



            glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
    }

}

//------------------------Draw the enemies-----------------------------------------

void enemyHandler (GLuint image) {
   
        saibaRender(image);
}

//--------------------enemy attack patterns----------------------------------------

void pattern_1 (Enemy &e)
{
        e.pos[0] -= 1;
        if (e.pos[0] < -50){
            e.pos[0] = g.xres;
            e.pos[1] = (rand() % (g.yres-100) + 1);
        }
}

void pattern_2 (Enemy &e)
{
    
        srand(time(NULL));
        //int start = (rand() % g.yres + 1);
        nticks+= 0.3;
        e.pos[0] -= 1;
        e.pos[1] = (50 * sin(nticks/50) + (e.wavepos));

        if (e.pos[0] < -50){
            e.pos[0] = g.xres;
            e.wavepos = (rand() % g.yres + 1);
            
        }
    
}

void pattern_3 (Enemy &e)
{
        e.pos[0] -= 1;
        e.pos[1] += 1;
        if (e.pos[0] < -50){
            e.pos[0] = g.xres;
            e.pos[1] = (rand() % g.yres + 1);
            
        }
}

void pattern_4 (Enemy &e) 
{
    e.pos[0] -= 1;
    e.pos[1] = (e.pos[0] * e.pos[0]);
    if (e.pos[0] < -50){
            e.pos[0] = g.xres;
            e.pos[1] = (rand() % g.yres + 1);
            
        }
}


