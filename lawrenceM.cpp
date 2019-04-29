/*
Software Engineering - 3350 
lawrenceM.cpp - source code for DBZ: The Final Form
Lawrence Marquez
Working on since: Februrary.

*/
#include "math.h"
#include <GL/glx.h>
#include "fonts.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include "Global.h"
#include "Timers.h"
#include "Image.h"
#include <iostream>
/* extern variables that are needed within my source code */
extern void sInit(GLuint,GLuint, GLuint);
extern void Enemy_init();
extern void Powerups_init();
extern int score_reset();
extern Global g; 
extern int gameState;
extern int done; 
extern int selectedOption;
extern Global walkTexture;
extern Timers timers;
double menuSelectionDelay = 0.15;
extern Image img[]; 
void renderControls();

using namespace std;

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

void newGame() 
{
  g.score = score_reset();
  sInit(g.walkTexture, g.ss3Texture, g.ssbTexture);
	Enemy_init();
	Powerups_init();
}
int acceptGameState(int selectedOption)
{
    //update game state to selected option in main menu
    switch (selectedOption) {
        case 0:
            gameState = INGAME;
            newGame();
            break;
        case 1:
            cout << "score" << endl;
            //make leaderboard function           
            break;
        case 2:
            renderControls();
            break;
        case 3:
            done = 1;
            return 1;
        case 4:
            gameState = INGAME;
            break;
        default:
            printf("FATAL ERROR IN GAME STATE\n\n");
            _exit(1);
    }
    return 0;
}
// void checkMouseMenu(XEvent *e)
// {
// 	//Did the mouse move?
// 	//Was a mouse button clicked?
// 	static int savex = -100;
// 	static int savey = -100;
// 	//
// 	if (e->type == ButtonRelease) {
// 		return;
// 	}
// 	if (e->type == ButtonPress) {
// 		if (e->xbutton.button==1) {
// 			//Left button is down
// 		}
// 		if (e->xbutton.button==3) {
// 			//Right button is down
// 		}
// 	}
// 	if (savex != e->xbutton.x || savey != e->xbutton.y) {
// 		//Mouse moved
// 		savex = e->xbutton.x;
// 		savey = e->xbutton.y;
// 	}
// }
void checkKeysLost()
{
    if (g.keys[XK_Up]) {
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
		&timers.timeCurrent);
	if (timeSpan > menuSelectionDelay) {
	    selectedOption = (((selectedOption-1)+4)%4);
	    timers.recordTime(&timers.menuSelectionTime);
	}
    }

    if (g.keys[XK_Down]) {
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
		&timers.timeCurrent);
	if (timeSpan > menuSelectionDelay) {
	    selectedOption = (((selectedOption+1)+4)%4);
	    timers.recordTime(&timers.menuSelectionTime);
	}
    }

    if (g.keys[XK_Return]) {
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
		&timers.timeCurrent);
	if (timeSpan > menuSelectionDelay) {
	    acceptGameState(selectedOption);
	    timers.recordTime(&timers.menuSelectionTime);
	}
    }
}

void checkKeysMainMenu() 
{
  if(g.keys[XK_Up]) {
    timers.recordTime(&timers.timeCurrent);
    double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
    if (timeSpan > menuSelectionDelay) {
      selectedOption = (((selectedOption - 1) + 4) % 4);
      timers.recordTime(&timers.menuSelectionTime);
    }
  }
  if(g.keys[XK_Down]) {
    timers.recordTime(&timers.timeCurrent);
    double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
    if(timeSpan > menuSelectionDelay) {
      selectedOption = (((selectedOption + 1) +4) % 4);
      timers.recordTime(&timers.menuSelectionTime);
    }
  }
  if(g.keys[XK_Return]) {
    timers.recordTime(&timers.timeCurrent);
    double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
    if (timeSpan > menuSelectionDelay) {
          acceptGameState(selectedOption);
          timers.recordTime(&timers.menuSelectionTime);
    }
  }
}

void checkKeysPauseMenu()
{
  if(g.keys[XK_Up]) {
    timers.recordTime(&timers.timeCurrent);
    double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
    if (timeSpan > menuSelectionDelay) {
      selectedOption = (((selectedOption - 1) + 5) % 5);
      timers.recordTime(&timers.menuSelectionTime);
    }
  }
  if(g.keys[XK_Down]) {
    timers.recordTime(&timers.timeCurrent);
    double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
    if(timeSpan > menuSelectionDelay) {
      selectedOption = (((selectedOption + 1) +5) % 5);
      timers.recordTime(&timers.menuSelectionTime);
    }
  }
  if(g.keys[XK_Return]) {
    timers.recordTime(&timers.timeCurrent);
    double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
    if (timeSpan > menuSelectionDelay) {
          acceptGameState(selectedOption);
          timers.recordTime(&timers.menuSelectionTime);
    }
  }
}

void renderMainMenuBackground(int x, int y, GLuint textInt) 
{
	glBindTexture(GL_TEXTURE_2D, textInt);
	glColor4f(1.0, 1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
	
  glTexCoord2f(0.0f, 1.0f); 
	glVertex2i(0, 35); //bottom left
	
  glTexCoord2f(0.0f, 0.0f); 
	glVertex2i(0, y); //top left

	glTexCoord2f(1.0f, 0.0f); 
	glVertex2i(x, y); //top right
	
  glTexCoord2f(1.0f, 1.0f); 
	glVertex2i(x, 35);

	glEnd();
}
// draw main logo
void renderMainMenu() {
  glPushMatrix();
  glColor4f(1.0, 1.0, 1.0, 1.0);
  glBindTexture(GL_TEXTURE_2D, g.finalFormLogoTexture);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.0f);
  glColor4ub(255,255,255,255);

  float ssWidth = 1;
  float ssHeight = 1;

  float textureX = 0;
  float textureY = 0;

  float centerX = g.xres/2;
  float centerY = g.yres*2/3; 

  float width = floor(((float)g.xres/2200)*img[11].width);
  float height = floor(((float)g.yres/1200)*img[11].height);

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

// display menu options
Rect r;
r.bot = g.yres/2;
r.left = g.xres/2;
r.center = 1;

switch (selectedOption) {
case 0:
  ggprint8b(&r, 16, 0x123fff, "NEW GAME");
  ggprint8b(&r, 16, 0xffffff, "SCORES");
  ggprint8b(&r, 16, 0xffffff, "CONTROLS");
  ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
  break;
case 1:
  ggprint8b(&r, 16, 0xffffff, "NEW GAME");
  ggprint8b(&r, 16, 0x123fff, "SCORES");
  ggprint8b(&r, 16, 0xffffff, "CONTROLS");
  ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
  break;
case 2:
  ggprint8b(&r, 16, 0xffffff, "NEW GAME");
  ggprint8b(&r, 16, 0xffffff, "SCORES");
  ggprint8b(&r, 16, 0x123fff, "CONTROLS");
  ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
  break;
case 3:
  ggprint8b(&r, 16, 0xffffff, "NEW GAME");
  ggprint8b(&r, 16, 0xffffff, "SCORES");
  ggprint8b(&r, 16, 0xffffff, "CONTROLS");
  ggprint8b(&r, 16, 0x123fff, "EXIT GAME");
  break;
default:
  break;
}
 
}

void renderPauseMenu()
{
  //draw final form logo 
  glPushMatrix();
  glColor4f(1.0, 1.0, 1.0, 1.0);
  glBindTexture(GL_TEXTURE_2D, g.finalFormLogoTexture);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.0f);
  glColor4ub(255,255,255,255);

  float ssWidth = 1;
  float ssHeight = 1;

  float textureX = 0;
  float textureY = 0;

  float centerX = g.xres/2;
  float centerY = g.yres*2/3; 

  float width = (((float)g.xres/2200)*img[11].width);
  float height = (((float)g.yres/1200)*img[11].height);

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

/* menu text */
  //display menu options
  Rect r;
  r.bot = g.yres/2;
  r.left = g.xres/2;
  r.center = 2;

  switch (selectedOption) {
      case 0:
          ggprint8b(&r, 16, 0xffffff, "RESUME GAME");
          ggprint8b(&r, 16, 0x123fff, "NEW GAME");
          ggprint8b(&r, 16, 0xffffff, "CONTROLS");
          ggprint8b(&r, 16, 0xffffff, "SCORES");
          ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
          break;
      case 1:
          ggprint8b(&r, 16, 0xffffff, "RESUME GAME");
          ggprint8b(&r, 16, 0xffffff, "NEW GAME");
          ggprint8b(&r, 16, 0x123fff, "CONTROLS");
          ggprint8b(&r, 16, 0xffffff, "SCORES");
          ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
          break;
      case 2:
          ggprint8b(&r, 16, 0xffffff, "RESUME GAME");
          ggprint8b(&r, 16, 0xffffff, "NEW GAME");
          ggprint8b(&r, 16, 0xffffff, "CONTROLS");
          ggprint8b(&r, 16, 0x123fff, "SCORES");
          ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
          break;
      case 3:
          ggprint8b(&r, 16, 0xffffff, "RESUME GAME");
          ggprint8b(&r, 16, 0xffffff, "NEW GAME");
          ggprint8b(&r, 16, 0xffffff, "CONTROLS");
          ggprint8b(&r, 16, 0xffffff, "SCORES");
          ggprint8b(&r, 16, 0x123fff, "EXIT GAME");
          break;
      case 4:
          ggprint8b(&r, 16, 0x123fff, "RESUME GAME");
          ggprint8b(&r, 16, 0xffffff, "NEW GAME");
          ggprint8b(&r, 16, 0xffffff, "CONTROLS");
          ggprint8b(&r, 16, 0xffffff, "SCORES");
          ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
          break;
      default:
          printf("FATAL GAME ERROR\n\n");
          break;
  }
}

void renderDeath()
{ 
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  //thinking of using an image for the death screen but i can just put you are dead 
  glPushMatrix();
  glColor3f(1.0, 1.0, 1.0);
  glBindTexture(GL_TEXTURE_2D, g.deathTexture);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.0f);
  glColor4ub(255,255,255,255);

  float ssWidth = 1;
  float ssHeight = 1;

  float textureX = 0;
  float textureY = 0;

  float centerX = g.xres/2;
  float centerY = g.yres*2/3; 

  float width = (((float)g.xres/400)*img[14].width);
  float height = (((float)g.yres/257)*img[14].height);

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

  // display menu options
Rect r;
r.bot = g.yres/3;
r.left = g.xres/2;
r.center = 1;

switch (selectedOption) {
  case 0:
    ggprint8b(&r, 16, 0x123fff, "NEW GAME");
    ggprint8b(&r, 16, 0xffffff, "SCORES");
    ggprint8b(&r, 16, 0xffffff, "CONTROLS");
    ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
    break;
  case 1: 
    ggprint8b(&r, 16, 0xffffff, "NEW GAME");
    ggprint8b(&r, 16, 0x123fff, "SCORES");
    ggprint8b(&r, 16, 0xffffff, "CONTROLS");
    ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
    break;
  case 2:
    ggprint8b(&r, 16, 0xffffff, "NEW GAME");
    ggprint8b(&r, 16, 0xffffff, "SCORES");
    ggprint8b(&r, 16, 0x123fff, "CONTROLS");
    ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
    break;
  case 3:
    ggprint8b(&r, 16, 0xffffff, "NEW GAME");
    ggprint8b(&r, 16, 0xffffff, "SCORES");
    ggprint8b(&r, 16, 0xffffff, "CONTROLS");
    ggprint8b(&r, 16, 0x123fff, "EXIT GAME");
    break;
  default:
    break;
}
}

void renderControls() {
  Rect r;

  unsigned int c = 0xffd700;
	r.bot = g.yres - 20;
	r.left = 10;
	r.center = 0;
 			
  ggprint8b(&r, 16, c, "K  Ki Blast");
  ggprint8b(&r, 16, c, "right arrow/d -> fly right");
  ggprint8b(&r, 16, c, "left arrow/a  <- fly left");
  ggprint8b(&r, 16, c, "up arrow/w -> fly up");
  ggprint8b(&r, 16, c, "down arrow/s -> fly down");
  ggprint8b(&r, 16, c, "esc - pause menu");
  ggprint8b(&r, 16, c, "frame: %i", g.walkFrame);

}
