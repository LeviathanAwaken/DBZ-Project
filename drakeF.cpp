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
#include "Global.h"
#include "Enemy.h"
#include "Boss.h"

extern Global g;
Enemy enemy[10];
Boss boss;
extern void enemyReference(Enemy *);
float nticks = 0.0;
int count = 5;
void Enemy_init();
void pattern_1(Enemy&);
void pattern_2(Enemy&);
void pattern_3(Enemy&);
void pattern_4(Enemy&);
int speed_Randomizer(void);
int freq_Randomizer(void);
int amp_Randomizer(void);
void detection();

class Explosion {
public:
	float pos[3];

	Explosion () {

		pos[0] = 0.0;
		pos[1] = 0.0;
		pos[2] = 0.0;
	}
} explosion;

//-----------------------credit screen stuff-----------------------------------------

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

//-----------------------------movement for enemies-----------------------------

void Enemy_init ()
{
	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		enemy[i].wavepos = (rand() % g.yres);
		int choice = (rand() % 3 + 1);
		enemy[i].xSpeed = speed_Randomizer();
		enemy[i].wavefreq = freq_Randomizer();
		enemy[i].waveamp = amp_Randomizer();
		enemy[i].pattern = choice;
		enemy[i].pos[0] = g.xres + (rand() % 100);
		enemy[i].pos[1] = (rand() % (g.yres));
		enemyReference(&enemy[i]);
	}

	boss.pos[0] = (g.xres + 100);
	boss.pos[1] = (g.yres/2);
	explosion.pos[0] = 5000;
	explosion.pos[1] = 0;
}

void saibaPhysics ()
{


	for (int i = 0; i < 10; i++) {
		if(enemy[i].pattern == 1)
			pattern_1(enemy[i]);
		if(enemy[i].pattern == 2)
			pattern_2(enemy[i]);
		//if(enemy[i].pattern == 3)
		//	pattern_3(enemy[i]);
		//if(enemy[i].pattern == 4)
		//	pattern_4(enemy[i]);
	}

}

void bossPhysics () 
{
	
	if (boss.pos[0] > g.xres/2) {
		boss.pos[0] -= 0.3;
	} 
}


//----------------------------Drawing the enemies-------------------------------------------------

//render the saibamen
void saibaRender (GLuint image)
{
	for (int i = 0; i < 10; i++) {
		glPushMatrix();
		glTranslated(enemy[i].pos[0], enemy[i].pos[1], enemy[i].pos[2]);
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, image);
		//
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);


			float tx = 0, ty = 0;

			
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

//render the boss
void bossRender (GLuint image)
{
	
		glPushMatrix();
		glTranslated(boss.pos[0], boss.pos[1], boss.pos[2]);
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, image);
		//
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);


			float tx = 0, ty = 0;

			
			glBegin(GL_QUADS);
			glTexCoord2f(tx+1, ty+1); glVertex2i(0, 0);
			glTexCoord2f(tx+1, ty);   glVertex2i(0, 150);
			glTexCoord2f(tx, ty);     glVertex2i(200, 150);
			glTexCoord2f(tx, ty+1);   glVertex2i(200, 0);
			glEnd();



			glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);


	

}

void explosionRender (GLuint image)
{
	glPushMatrix();
		glTranslated(explosion.pos[0], explosion.pos[1], explosion.pos[2]);
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, image);
		//
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);


			float tx = 0, ty = 0;

			
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

//------------------------Draw the enemies-----------------------------------------

void enemyHandler (GLuint image1, GLuint image2, GLuint image3) {

		saibaRender(image1);
		bossRender(image2);
		explosionRender(image3);
}

//--------------------enemy attack patterns----------------------------------------

void pattern_1 (Enemy &e)
{
		e.pos[0] -= e.xSpeed;
		if (e.pos[0] < -50){
			e.pos[0] = g.xres;
			e.pos[1] = (rand() % (g.yres - 100) + 1);
			e.xSpeed = speed_Randomizer();
		}
}

void pattern_2 (Enemy &e)
{
		
		srand(time(NULL));
		
		nticks+= 0.3;
		e.pos[0] -= e.xSpeed;
		e.pos[1] = (e.waveamp * sin(nticks/e.wavefreq) + (e.wavepos));

		if (e.pos[0] < -50){
			e.pos[0] = g.xres;
			e.wavepos = (rand() % (g.yres) + 1);
			e.xSpeed = speed_Randomizer();
			e.wavefreq = freq_Randomizer();
			e.waveamp = amp_Randomizer();

		}

}

void pattern_3 (Enemy &e)
{
	e.pos[1] = g.yres;
	e.pos[0] -= e.xSpeed;
	e.pos[1] -= 2;
	
	if (e.pos[0] < -50){
		e.pos[0] = g.xres;		
		e.xSpeed = speed_Randomizer();
	}
}

void pattern_4 (Enemy &e)
{
	e.pos[1] = 0.0;
	e.pos[0] -= e.xSpeed;
	e.pos[1] += 2;
	
	if (e.pos[0] < -50){
		e.pos[0] = g.xres;		
		e.xSpeed = speed_Randomizer();
	}
}

//------------------variable randomization------------------------------

/* variables include horizontal translation speed, sine wave
 * frequency, and sine wave amplitude
 */

int speed_Randomizer (void)
{
	int speed = (rand() % 3 + 2);
	return speed;
}

int freq_Randomizer (void)
{
	int freq = ((rand() % 31 + 1) + 19);
	return freq;
}

int amp_Randomizer (void)
{
	int amp = ((rand() % 3 + 5) * 10);
	return amp;
}


//-------------------collision detection---------------------------------------

/*Enemy collision detection for whenever an enemy is hit by a ki blast
 *or collides with goku directly
 */

void detection (int Eindices) {
	enemy[Eindices].eHealth --;
	if (enemy[Eindices].eHealth == 0) {
		explosion.pos[0] = enemy[Eindices].pos[0];
		explosion.pos[1] = enemy[Eindices].pos[1];
		enemy[Eindices].pos[0] = g.xres;
		enemy[Eindices].pos[1] = (rand() % (g.yres - 100) + 1);
		enemy[Eindices].wavepos = (rand() % (g.yres) + 1);
		enemy[Eindices].xSpeed = speed_Randomizer();
		enemy[Eindices].wavefreq = freq_Randomizer();
		enemy[Eindices].waveamp = amp_Randomizer();
		enemy[Eindices].eHealth = 2;
	}

} 