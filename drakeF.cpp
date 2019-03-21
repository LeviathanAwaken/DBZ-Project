//3350
//Program:  drakeF.cpp
//Author:   Drake Floyd
//Date:     2/15/19


#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
//extern Global g;

typedef double Vec[3];

/*class global {
public:
    int xres, yres;
    GLuint image;

} glb;*/

class Enemy {
public:
    
    float pos[3] = {0.0, 0.0, 0.0};

    //Vec saiba[3];
    Enemy () {
     
     srand(time(0));
     /*for (int i = 0; i < 1; i++) {
        saiba[i][0] = 800;
        saiba[i][1] = (rand() % 500 + 1);
        saiba[i][2] = 0.0;
     }*/
     pos[0] = 800;
     pos[1] = (rand() % 500 + 1);
     pos[2] = 0.0;
    }
}Enemy;




//Enemy saiba[3];


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

//movement for enemy work in progress
void saibaPhysics () 
{
    //float move = 3.14;
    
    
    Enemy.pos[0] -= 2;
    /*for (int i = 0; i < 5; i++){
        Enemy.pos[1] += sin(i * move/2) * 300;
    }*/
        
    if (Enemy.pos[0] < -50){
        Enemy.pos[0] = 820;
        Enemy.pos[1] = (rand() % 500 +1);
    }
        
    
    

}

void saibaRender (GLuint image) 
{
    for (int i = 0; i < 1; i++) {
        glPushMatrix();
        glTranslated(Enemy.pos[0], Enemy.pos[1], Enemy.pos[2]);
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

//handler for amount of enemies on screen

void enemyHandler (GLuint image) {
    //for (int i = 0; i < 3; i++) {
        saibaRender(image);
   // }
}


