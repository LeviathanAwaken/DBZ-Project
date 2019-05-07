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
#include "Image.h"
#include <vector>

extern Global g;
extern Image img[];
const int count = 15;
Enemy enemy[count];
Boss boss;
extern void enemyReference(Enemy *);
extern void bossReference(Boss *);
extern void bossCollision();
extern void saibaCollision();
extern void score_update(int);
float nticks = 0.0;
void Enemy_init();
void pattern_1(Enemy&);
void pattern_2(Enemy&);
void pattern_3(Enemy&);
void pattern_4(Enemy&);
int speed_Randomizer(void);
int freq_Randomizer(void);
int amp_Randomizer(void);
void detection();
void difficulty(Enemy&);
void blastInit();
//int healthMod = 0;


//-----------------explosion class------------------------------------------------
class Explosion {
public:
	float centerX;
	float centerY;
	float height;
	float width;
	int spriteSheetIndex;
	int frame;
	bool done;
	Explosion(float, float);
	void draw();
};

std::vector<Explosion> explosions;

class BlastDwn {
public:
	float centerX;
	float centerY;
	float height;
	float width;
	int spriteSheetIndex;
	int frame;
	bool live;
	void draw();
}blastDwn[4];

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
	for (int i = 0; i < count; i++) {
		enemy[i].wavepos = ((rand() % (g.yres/2))+100);
		int choice = (rand() % 4 + 1);
		enemy[i].xSpeed = speed_Randomizer();
		enemy[i].wavefreq = freq_Randomizer();
		enemy[i].waveamp = amp_Randomizer();
		enemy[i].pattern = choice;
		enemy[i].pos[0] = g.xres + ((rand() % 100) + 100);
		enemy[i].pos[1] = (rand() % (g.yres));
		enemy[i].eHealth = 1;
		enemy[i].image = g.saibaTexture;
		enemyReference(&enemy[i]);
	}
	boss.eHealth = 50;
	boss.pos[0] = (g.xres + 200);
	boss.pos[1] = (g.yres/2);
	bossReference(&boss);
	blastInit();

}

void blastInit() {
	int space = 0;
	for (int i = 0; i < 4; i++) {
		blastDwn[i].centerX = -20 + space;
		blastDwn[i].centerY = (g.yres+20) + space; /*(rand() % 300 + 100)*/
		blastDwn[i].height = img[28].height / 0.5;
		blastDwn[i].width = img[28].width / (6 * 1.5);
		blastDwn[i].spriteSheetIndex = 30;
		blastDwn[i].frame = 0;
		blastDwn[i].live = false;
		space+= 200;
	}
}

void saibaPhysics ()
{

	for (int i = 0; i < count; i++) {
		if(enemy[i].pattern == 1)
			pattern_1(enemy[i]);
		if(enemy[i].pattern == 2)
			pattern_2(enemy[i]);
		if(enemy[i].pattern == 3)
			pattern_3(enemy[i]);
		if(enemy[i].pattern == 4)
			pattern_4(enemy[i]);
	}
	saibaCollision();
}

void bossPhysics ()
{

	if (g.score >= 5000) {
		if (boss.eHealth > 0) {	
			if (boss.pos[0] > g.xres/2) {
				boss.pos[0] -= 1.0;
				if (boss.pos[0] < g.xres + 100) {
					boss.isRendered = true;
				}

			}
		} else if (boss.eHealth <= 0){
			boss.pos[0] +=3.5;
			if (boss.pos[0] > g.xres + 1000) {
				boss.eHealth = 70;
			}
		}
		nticks+= 0.3;
		boss.pos[1] = (70 * sin(nticks/50) + (g.yres/2));
		bossCollision();
		if ((boss.pos[0] == g.xres/2) && (boss.eHealth > 50)) {
			for (int i = 0; i < 4; i++) {
				blastDwn[i].live = true;
			}
		}
	}
}


//----------------------------Drawing the enemies-------------------------------------------------

//render the saibamen
void saibaRender ()
{
	for (int i = 0; i < count; i++) {
		enemy[i].isRendered = true;
		glPushMatrix();
		glTranslated(enemy[i].pos[0], enemy[i].pos[1], enemy[i].pos[2]);
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, enemy[i].image);
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
		glColor3f(0.0, 0.0, 0.0);
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


void explosionRender ()
{
	for (unsigned int i = 0; i < explosions.size(); i++) {
		explosions[i].draw();
	}
}

Explosion::Explosion (float x, float y)
{
	centerX = x;
	centerY = y;
	height = .1* (float)g.yres;
	width = height;
	int exp = rand() % 3;
	if (exp == 0) {
		spriteSheetIndex = 16;
	}
	else if (exp == 1) {
		spriteSheetIndex = 13;
	}
	else {
		spriteSheetIndex = 17;
	}
	frame = 0;
	done = false;
}

void Explosion::draw()
{
	GLuint temp;
	if (spriteSheetIndex == 16) {
		temp = g.explosion2Texture;
	} else if (spriteSheetIndex == 13) {
		temp = g.explosionTexture;
	} else {
		temp = g.explosion3Texture;
	}
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, temp);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);

	float ssWidth = (float)1.0/img[spriteSheetIndex].columns;
	float ssHeight = (float)1.0/img[spriteSheetIndex].rows;

	int ix = this->frame % img[spriteSheetIndex].columns;
	int iy = 0;

	if (frame >= img[spriteSheetIndex].columns) {
		iy = 1;
	}


	if (frame >= (img[spriteSheetIndex].columns*2)) {
		iy = 2;
	}

	if (frame >= (img[spriteSheetIndex].columns*3)) {
		iy = 3;
	}

	if (frame >= img[spriteSheetIndex].columns*4) {
		iy = 4;
	}

	if (frame >= (img[spriteSheetIndex].columns*5)) {
		iy = 5;
	}

	if (frame >= (img[spriteSheetIndex].columns*6)) {
		iy = 6;
	}

	if (frame >= img[spriteSheetIndex].columns*7) {
		iy = 7;
	}

	if (frame >= img[spriteSheetIndex].columns*8) {
		iy = 8;
	}

	float textureX = (float)ix / img[spriteSheetIndex].columns;
	float textureY = (float)iy / img[spriteSheetIndex].rows;

	glBegin(GL_QUADS);
	glTexCoord2f(textureX, textureY+ssHeight);
	glVertex2i(centerX-width, centerY-height);

	glTexCoord2f(textureX, textureY);
	glVertex2i(centerX-width, centerY+height);

	glTexCoord2f(textureX+ssWidth, textureY);
	glVertex2i(centerX+width, centerY+height);

	glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
	glVertex2i(centerX+width, centerY-height);
	glEnd();

	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);

	//advance frame
	frame++;
	if(frame >= img[spriteSheetIndex].columns*8){
		frame = 0;
		done = true;
	}

}

void blastRender ()
{
	for (int i = 0; i < 4; i++) {
		blastDwn[i].draw();
	}
}

/*BlastDwn::Bblast (float x, float y)
{
	centerX = x;
	centerY = y;
	height = .1* (float)g.yres;
	width = height;
	//BlastDwn.pos[0] = (g.xres/2);
	//BlastDwn.pos[1] = g.yres - 100;
	//BlastDwn.pos[2] = 0.0;
	spriteSheetIndex = 28;
	frame = 0;
	done = false;
}*/

void BlastDwn::draw()
{
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, g.energy_downTexture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);

		float ssWidth = (float)1.0/img[spriteSheetIndex].columns;
		float ssHeight = (float)1.0/img[spriteSheetIndex].rows;

		int ix = this->frame % img[spriteSheetIndex].columns;
		int iy = 0;

		if (frame >= img[spriteSheetIndex].columns) {
			iy = 1;
		}


		if (frame >= (img[spriteSheetIndex].columns*2)) {
			iy = 2;
		}

		if (frame >= (img[spriteSheetIndex].columns*3)) {
			iy = 3;
		}

		if (frame >= img[spriteSheetIndex].columns*4) {
			iy = 4;
		}

		if (frame >= (img[spriteSheetIndex].columns*5)) {
			iy = 5;
		}

		if (frame >= (img[spriteSheetIndex].columns*6)) {
			iy = 6;
		}

		if (frame >= img[spriteSheetIndex].columns*7) {
			iy = 7;
		}

		if (frame >= img[spriteSheetIndex].columns*8) {
			iy = 8;
		}

		float textureX = (float)ix / img[spriteSheetIndex].columns;
		float textureY = (float)iy / img[spriteSheetIndex].rows;

		glBegin(GL_QUADS);
		glTexCoord2f(textureX, textureY+ssHeight);
		glVertex2i(centerX-width, centerY-height);

		glTexCoord2f(textureX, textureY);
		glVertex2i(centerX-width, centerY+height);

		glTexCoord2f(textureX+ssWidth, textureY);
		glVertex2i(centerX+width, centerY+height);

		glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
		glVertex2i(centerX+width, centerY-height);
		glEnd();

		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);

		//advance frame
		frame++;
		if(frame >= img[spriteSheetIndex].columns*8){
			frame = 0;
			//done = true;
		}

}

void blastPhysics () {
	
	for (int i = 0; i < 4; i++) {
		if (blastDwn[i].live) {
			blastDwn[i].centerX += 0.5;
			blastDwn[i].centerY -= 1;
			if (blastDwn[3].centerY < 0) {
				blastInit();
			}
		}
	}
}

//------------------------Draw the enemies-----------------------------------------

void enemyHandler (GLuint image1) {

	saibaRender();
	if (boss.isRendered) {
		bossRender(image1);
	}

}

//--------------------enemy attack patterns----------------------------------------

void pattern_1 (Enemy &e)
{

	e.pos[0] -= e.xSpeed+1;
	if (e.pos[0] < -50){
		e.pos[0] = g.xres;
		e.pos[1] = (rand() % (g.yres - 100) + 1);
		e.xSpeed = speed_Randomizer();
	}
}

void pattern_2 (Enemy &e)
{

	nticks+= 0.3;
	e.pos[0] -= e.xSpeed;
	e.pos[1] = (e.waveamp * sin(nticks/e.wavefreq) + (e.wavepos));

	if (e.pos[0] < -50){
		e.pos[0] = g.xres;
		e.wavepos = ((rand() % (g.yres/2)) + 100);
		e.xSpeed = speed_Randomizer();
		e.wavefreq = freq_Randomizer();
		e.waveamp = amp_Randomizer();
	}

}

void pattern_3 (Enemy &e)
{

	e.pos[0] -= (e.xSpeed+2);
	e.pos[1] -= 3.0;

	if (e.pos[0] < -50){
		e.pos[0] = g.xres;
		e.xSpeed = speed_Randomizer();
		e.pos[1] = (rand() % (g.yres));
	}
}

void pattern_4 (Enemy &e)
{

	nticks+= 0.3;
	e.pos[0] -= e.xSpeed;
	e.pos[1] = (e.waveamp * cos(nticks/e.wavefreq) + (e.wavepos));

	if (e.pos[0] < -50){
		e.pos[0] = g.xres;
		e.wavepos = ((rand() % (g.yres/2)) + 100);
		e.xSpeed = speed_Randomizer();
		e.wavefreq = freq_Randomizer();
		e.waveamp = amp_Randomizer();
	}

}

//------------------variable randomization------------------------------

/* variables include horizontal translation speed, sine wave
 * frequency, and sine wave amplitude
 */

int speed_Randomizer (void)
{
	float mod = 2.5;
	if (g.score >= 4000) {
		mod = 3;
	}
	if (g.score >= 5000) {
		mod = 4;
	}
	if (g.score >= 6000) {
		mod = 5;
	}
	int speed = (rand() % 3 + mod);
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


//--------------------------Explosion Logic-------------------------------

void createExplosion (float x, float y)
{
	Explosion temp(x,y);
	explosions.push_back(temp);
}

void cleanExplosions ()
{
	for (int i = explosions.size()-1; i >= 0; i--) {
		if (explosions[i].done) {
			explosions.erase(explosions.begin()+i);
		}
	}
}

//------------------------difficulty increase logic----------------------------

void difficulty(Enemy &e)
{
	if (g.score >= 3000 && g.score < 4000) {
		e.eHealth = 2;
	}
	if (g.score >= 4000 && g.score < 5000) {
		e.eHealth = 3;
	}
	if (g.score >= 5000) {
		e.eHealth = 4;
	}

}
//-------------------collision detection---------------------------------------

/*Enemy collision detection for whenever an enemy is hit by a ki blast
 *or collides with goku directly
 */

void detection (int Eindices, bool type)
{
	if (type == false) {
		if (enemy[Eindices].isRendered) {
			enemy[Eindices].eHealth --;
		}
	} else {
		if (enemy[Eindices].isRendered) {
			enemy[Eindices].eHealth = 0;
		}
	}
		if (enemy[Eindices].eHealth <= 0) {
			createExplosion(enemy[Eindices].pos[0], enemy[Eindices].pos[1]);
			enemy[Eindices].pos[0] = g.xres;
			enemy[Eindices].pos[1] = (rand() % (g.yres - 100) + 1);
			enemy[Eindices].wavepos = (rand() % (g.yres - 100) + 1);
			enemy[Eindices].xSpeed = speed_Randomizer();
			enemy[Eindices].wavefreq = freq_Randomizer();
			enemy[Eindices].waveamp = amp_Randomizer();
			if (g.score >= 5000) {
				enemy[Eindices].image = g.redSaibaTexture;
			}
			difficulty(enemy[Eindices]);
			score_update(100);
		}


}

void bossDetection ()
{

	boss.eHealth --;
	createExplosion(boss.pos[0], boss.pos[1]);
	if (boss.eHealth <= 0) {
		createExplosion(boss.pos[0], boss.pos[1]);
		createExplosion(boss.pos[0] + 50, boss.pos[1]);
		createExplosion(boss.pos[0] + 50, boss.pos[1] + 50);
		createExplosion(boss.pos[0], boss.pos[1] + 50);
		//boss.pos[0] = 5000;
		//boss.eHealth = 120;
		score_update(10000);
	}

}
