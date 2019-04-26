//3350
// Lawrence Marquez
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
extern Global g; // global declaration
// extern Timers timers; //timer class
extern int keys[];
extern int gameState;
extern int done;
extern int cursorPosition[];
extern int selectedOption;
extern Global walkTexture;
extern Timers timers;
extern double menuSelectionDelay = 0.15;
extern Image img[];


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

  //setup new game when option is selected
}
int acceptGameState(int selectedOption)
{
    //update game state to selected option in main menu
    switch (selectedOption) {
        case 0:
            // newGame();
            gameState = INGAME;
            break;
        case 1:
            //make leaderboard function           
            break;
        case 2:
            done=1;
            return 1;
        case 3:
            gameState = INGAME;
            break;
        default:
            printf("FATAL ERROR IN GAME STATE\n\n");
            _exit(1);
    }
    return 0;
}
void checkMouseMenu(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = -100;
	static int savey = -100;
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
}
void checkKeysLost()
{
    if (keys[XK_Up]) {
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
		&timers.timeCurrent);
	if (timeSpan > menuSelectionDelay) {
	    selectedOption = ((selectedOption-1)+3)%3;
	    timers.recordTime(&timers.menuSelectionTime);
	}
    }

    if (keys[XK_Down]) {
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
		&timers.timeCurrent);
	if (timeSpan > menuSelectionDelay) {
	    selectedOption = ((selectedOption+1)+3)%3;
	    timers.recordTime(&timers.menuSelectionTime);
	}
    }

    if (keys[XK_Return]) {
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
  if(keys[XK_Up]) {
    timers.recordTime(&timers.timeCurrent);
    double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
    if (timeSpan > menuSelectionDelay) {
      selectedOption = (((selectedOption - 1) + 3) % 3);
      timers.recordTime(&timers.menuSelectionTime);
    }
  }
  if(keys[XK_Down]) {
    timers.recordTime(&timers.timeCurrent);
    double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
    if(timeSpan > menuSelectionDelay) {
      selectedOption = (((selectedOption + 1) +3) % 3);
      timers.recordTime(&timers.menuSelectionTime);
    }
  }
  if(keys[XK_Return]) {
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
  if(keys[XK_Up]) {
    timers.recordTime(&timers.timeCurrent);
    double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
    if (timeSpan > menuSelectionDelay) {
      selectedOption = (((selectedOption - 1) + 4) % 4);
      timers.recordTime(&timers.menuSelectionTime);
    }
  }
  if(keys[XK_Down]) {
    timers.recordTime(&timers.timeCurrent);
    double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
    if(timeSpan > menuSelectionDelay) {
      selectedOption = (((selectedOption + 1) +4) % 4);
      timers.recordTime(&timers.menuSelectionTime);
    }
  }
  if(keys[XK_Return]) {
    timers.recordTime(&timers.timeCurrent);
    double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
    if (timeSpan > menuSelectionDelay) {
          acceptGameState(selectedOption);
          timers.recordTime(&timers.menuSelectionTime);
    }
  }
}
// extern void setBackgroundNamek(int, int, GLuint);

void renderMainMenu() 
{
  // setBackgroundNamek(g.xres, g.yres, g.namekTexture);

  //set background to game background
  glPushMatrix();
  glColor3f(1.0, 1.0, 1.0);
  glBindTexture(GL_TEXTURE_2D, g.namekTexture);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.0f);
  glColor4ub(255, 255, 255, 255);

  //working on menu placement
  float ssWidth = (float)1.0/img[8].width;
  float ssHeight = (float)1.0/img[8].height;

  float textureX = 0;
  float textureY = 0;

  float centerX = g.xres/2;
  float centerY = (g.yres/2) + g.yres *0.25;

  float width = img[8].width/1.6;
  float height = img[8].height/1.5;

  glBegin(GL_QUADS);

  glTexCoord2f(textureX, textureY + ssHeight);
  glVertex2i(centerX - width, centerY - height);

  glTexCoord2f(textureX, textureY);
  glVertex2i(centerX -width, centerY + height);

  glTexCoord2f(textureX + ssWidth, textureY + ssHeight);
  glVertex2i(centerX + width, centerY - height);
  glEnd();

 
// draw main logo
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.finalFormLogoTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    ssWidth = (float)1.0/img[11].width;
    ssHeight = (float)1.0/img[11].height;

    textureX = 0;
    textureY = 0;

    centerX = g.xres/2;
    centerY = g.yres*2/3; 

    width = floor(((float)g.xres/2200)*img[11].width);
    height = floor(((float)g.yres/1200)*img[11].height);

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
    ggprint8b(&r, 16, 0x123fff, "START A NEW GAME");
    ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
    break;
  case 1:
    ggprint8b(&r, 16, 0xffffff, "START A NEW GAME");
    ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
    break;
  case 2:
    ggprint8b(&r, 16, 0xffffff, "START A NEW GAME");
    ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
    break;
  default:
    break;
}
 
}

void renderPauseMenu()
{
    //draw final form logo //////////////////////////////////////
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
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

    ////////////////////////////////////////////

    //display meny options
    Rect r;
    r.bot = g.yres/3;
    r.left = g.xres/2;
    r.center = 1;

    switch (selectedOption) {
        case 0:
            // if (walkTexture.centerY!=800) {
                ggprint8b(&r, 16, 0xffffff, "RESUME GAME");
            // }
            ggprint8b(&r, 16, 0x123fff, "NEW GAME");
            ggprint8b(&r, 16, 0xffffff, "EXIT");
            break;
        case 1:
            // if (walkTexture.centerY!=800) {
                ggprint8b(&r, 16, 0xffffff, "RESUME GAME");
            // }
            ggprint8b(&r, 16, 0xffffff, "NEW GAME");
            ggprint8b(&r, 16, 0xffffff, "EXIT");
            break;
        case 2:
            // if (walkTexture.centerY!=800) {
                ggprint8b(&r, 16, 0xffffff, "RESUME GAME");
            // }
            ggprint8b(&r, 16, 0xffffff, "NEW GAME");
            ggprint8b(&r, 16, 0x123fff, "EXIT");
            break;
        case 3:
            // if (walkTexture.centerY!=800) {
                ggprint8b(&r, 16, 0x123fff, "RESUME GAME");
            // }
            ggprint8b(&r, 16, 0xffffff, "NEW GAME");
            ggprint8b(&r, 16, 0xffffff, "EXIT");
            break;
        default:
            // printf("FATAL GAME ERROR\n\n");
            break;
    }
}

// void renderDeath()
// { //thinking of using an image for the death screen but i can just put you are dead 
//   glPushMatrix();
//   glColor3f(1.0, 1.0, 1.0);
//   glBindTexture(GL_TEXTURE_2D, g.deathTexture);
//   glEnable(GL_ALPHA_TEST);
//   glAlphaFunc(GL_GREATER, 0.0f);
//   glColor4ub(255,255,255,255);

//   float ssWidth = (float)1.0/img[14].width;
//   float ssHeight = (float)1.0/img[14].height;

//   float textureX = 0;
//   float textureY = 0;

//   float centerX = g.xres/2;
//   float centerY = g.yres*2/3; 

//   float width = floor(((float)g.xres/2200)*img[14].width);
//   float height = floor(((float)g.yres/1200)*img[14].height);

//   glBegin(GL_QUADS);
//   glTexCoord2f(textureX, textureY+ssHeight);
//   glVertex2i(centerX-width, centerY-height);

//   glTexCoord2f(textureX, textureY);
//   glVertex2i(centerX-width, centerY+height);

//   glTexCoord2f(textureX+ssWidth, textureY);
//   glVertex2i(centerX+width, centerY+height);

//   glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
//   glVertex2i(centerX+width, centerY-height);
//   glEnd();

//   glPopMatrix();
//   glBindTexture(GL_TEXTURE_2D, 0);
//   glDisable(GL_ALPHA_TEST);
// }
