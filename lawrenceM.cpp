//3350
// Lawrence Marquez

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

extern Global g; // global declaration
// extern Timers timers; //timer class
extern int keys[];
extern int gameState;
extern int done;
extern int cursorPosition[];
extern int selectedOption;
extern Global walkTexture;
extern Timers timers;
// extern int SpriteSheet img[];
double menuSelectionDelay = 0.15;


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

// void checkMouseMainMenu(XEvent *e) {
//   //Did the mouse move or has the mouse been clicked?
//   static int savex = -100;
//   static int savey = -100;

//   if (e-> type == ButtonRelease) {
//     return;
//   }
//   if (e->type == ButtonPress) {
//     if (e->xbutton.button == 1) {
//         //Left Button is pressed
//     }
//     if (e->xbutton.button == 3) {
//       //Right button is pressed
//     }
//   }
//   if(savex != e->xbutton.x || savey != e->xbutton.y) {
//     // Mouse has been moved
//     savex = e->xbutton.x;
//     savey = e->xbutton.y;
//   }
//   cursorPosition[0] = savex;
//   cursorPosition[1] = savey;
// }

// void checkKeysMainMenu() 
// {
//   if(keys[XK_Up]) {
//     timers.recordTime(&timers.timeCurrent);
//     double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
//     if (timeSpan > menuSelectionDelay) {
//       selectedOption = (((selectedOption - 1) + 3) % 3);
//       timers.recordTime(&timers.menuSelectionTime);
//     }
//   }
//   if(keys[XK_Down]) {
//     timers.recordTime(&timers.timeCurrent);
//     double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
//     if(timeSpan > menuSelectionDelay) {
//       selectedOption = (((selectedOption + 1) +3) % 3);
//       timers.recordTime(&timers.menuSelectionTime);
//     }
//   }
//   if(keys[XK_Return]) {
//     timers.recordTime(&timers.timeCurrent);
//     double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
//     if (timeSpan > menuSelectionDelay) {
//           acceptGameState(selectedOption);
//           timers.recordTime(&timers.menuSelectionTime);
//     }
//   }
// }

// // void checkKeysPauseMenu()
// // {
// //   if(keys[XK_Up]) {
// //     timers.recordTime(&timers.timeCurrent);
// //     double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
// //     if (timeSpan > menuSelectionDelay) {
// //       selectedOption = (((selectedOption - 1) + 4) % 4);
// //       timers.recordTime(&timers.menuSelectionTime);
// //     }
// //   }
// //   if(keys[XK_Down]) {
// //     timers.recordTime(&timers.timeCurrent);
// //     double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
// //     if(timeSpan > menuSelectionDelay) {
// //       selectedOption = (((selectedOption + 1) +4) % 4);
// //       timers.recordTime(&timers.menuSelectionTime);
// //     }
// //   }
// //   if(keys[XK_Return]) {
// //     timers.recordTime(&timers.timeCurrent);
// //     double timeSpan = timers.timeDiff(&timers.menuSelectionTime, &timers.timeCurrent);
// //     if (timeSpan > menuSelectionDelay) {
// //           acceptGameState(selectedOption);
// //           timers.recordTime(&timers.menuSelectionTime);
// //     }
// //   }
// // }

// void renderMainMenu() 
// {
//   //set background to game background
//   glPushMatrix();
//   glColor3f(1.0, 1.0, 1.0);
//   glBindTexture(GL_TEXTURE_2D, g.namekTexture);
//   glEnable(GL_ALPHA_TEST);
//   glAlphaFunc(GL_GREATER, 0.0f);
//   glColor4ub(255, 255, 255, 255);

//   //working on menu placement
//   float ssWidth;
//   float ssHeight;

//   float textureX = 0;
//   float textureY = 0;

//   float centerX = g.xres/2;
//   float centerY = g.yres*2/3;

//   float width;
//   float height;

//   glBegin(GL_QUADS);

//   glTexCoord2f(textureX, textureY + ssHeight);
//   glVertex2i(centerX - width, centerY - height);

//   glTexCoord2f(textureX, textureY);
//   glVertex2i(centerX -width, centerY + height);

//   glTexCoord2f(textureX + ssWidth, textureY + ssHeight);
//   glVertex2i(centerX + width, centerY - height);
//   glEnd();

//   glPopMatrix();
//   glBindTexture(GL_TEXTURE_2D, 0);
//   glDisable(GL_ALPHA_TEST);

//   // gonna work on a main logo possibly but might just print the name in big letters

//   glPushMatrix();
//   glColor3f(1.0, 1.0, 1.0);
//   glBindTexture(GL_TEXTURE_2D, g.finalFormLogoTexture);
//   glEnable(GL_ALPHA_TEST);
//   glAlphaFunc(GL_GREATER, 0.0f);
//   glColor4ub(255, 255, 255, 255);

  
// // display menu options
// Rect r;
// r.bot = g.yres/3;
// r.left = g.xres/2;
// r.center = 1;

// switch (selectedOption) {
//   case 0:
//     ggprint8b(&r, 16, 0x123fff, "START A NEW GAME");
//     ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
//     break;
//   case 1:
//     ggprint8b(&r, 16, 0xffffff, "START A NEW GAME");
//     ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
//     break;
//   case 2:
//     ggprint8b(&r, 16, 0xffffff, "START A NEW GAME");
//     ggprint8b(&r, 16, 0xffffff, "EXIT GAME");
//     break;
//   default:
//     break;
// }
//   // glPushMatrix();
//   //   glColor3f(1.0, 1.0, 1.0);
//   //   glBindTexture(GL_TEXTURE_2D, g.cursorTexture);
//   //   glEnable(GL_ALPHA_TEST);
//   //   glAlphaFunc(GL_GREATER, 0.0f);
//   //   glColor4ub(255,255,255,255);

//     // ssWidth = (float)1.0/img[7].columns;
//     // ssHeight = (float)1.0/img[7].rows;

//     // textureX = 0;
//     // textureY = 0;

//     // centerX = cursorPosition[0];
//     // centerY = g.yres - cursorPosition[1];

//     // width = (img[7].width/img[7].columns)*0.5;
//     // height = (img[7].height/img[7].rows)*0.5;

//     // glBegin(GL_QUADS);
//     // glTexCoord2f(textureX, textureY+ssHeight);
//     // glVertex2i(centerX-width, centerY-height);

//     // glTexCoord2f(textureX, textureY);
//     // glVertex2i(centerX-width, centerY+height);

//     // glTexCoord2f(textureX+ssWidth, textureY);
//     // glVertex2i(centerX+width, centerY+height);

//     // glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
//     // glVertex2i(centerX+width, centerY-height);
//     // glEnd();

//     // glPopMatrix();
//     // glBindTexture(GL_TEXTURE_2D, 0);
//     // glDisable(GL_ALPHA_TEST);

//     ////////////////////////////////////////////
// }

// void renderPauseMenu()
// {
//     //draw final form logo //////////////////////////////////////
//     glPushMatrix();
//     glColor3f(1.0, 1.0, 1.0);
//     glBindTexture(GL_TEXTURE_2D, g.finalFormLogoTexture);
//     glEnable(GL_ALPHA_TEST);
//     glAlphaFunc(GL_GREATER, 0.0f);
//     glColor4ub(255,255,255,255);

//     float ssWidth = 10;
//     float ssHeight = 10;

//     float textureX = 0;
//     float textureY = 0;

//     float centerX = g.xres/2;
//     float centerY = g.yres*2/3; 

//     float width = (((float)g.xres/1280)*200);
//     float height = (((float)g.yres/720)*200);

//     glBegin(GL_QUADS);
//     glTexCoord2f(textureX, textureY+ssHeight);
//     glVertex2i(centerX-width, centerY-height);

//     glTexCoord2f(textureX, textureY);
//     glVertex2i(centerX-width, centerY+height);

//     glTexCoord2f(textureX+ssWidth, textureY);
//     glVertex2i(centerX+width, centerY+height);

//     glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
//     glVertex2i(centerX+width, centerY-height);
//     glEnd();

//     glPopMatrix();
//     glBindTexture(GL_TEXTURE_2D, 0);
//     glDisable(GL_ALPHA_TEST);

//     ////////////////////////////////////////////

//     //display meny options
//     Rect r;
//     r.bot = g.yres/3;
//     r.left = g.xres/2;
//     r.center = 1;

//     switch (selectedOption) {
//         case 0:
//             // if (walkTexture.centerY!=800) {
//                 ggprint8b(&r, 16, 0xffffff, "RESUME GAME");
//             // }
//             ggprint8b(&r, 16, 0x123fff, "NEW GAME");
//             ggprint8b(&r, 16, 0xffffff, "EXIT");
//             break;
//         case 1:
//             // if (walkTexture.centerY!=800) {
//                 ggprint8b(&r, 16, 0xffffff, "RESUME GAME");
//             // }
//             ggprint8b(&r, 16, 0xffffff, "NEW GAME");
//             ggprint8b(&r, 16, 0xffffff, "EXIT");
//             break;
//         case 2:
//             // if (walkTexture.centerY!=800) {
//                 ggprint8b(&r, 16, 0xffffff, "RESUME GAME");
//             // }
//             ggprint8b(&r, 16, 0xffffff, "NEW GAME");
//             ggprint8b(&r, 16, 0x123fff, "EXIT");
//             break;
//         case 3:
//             // if (walkTexture.centerY!=800) {
//                 ggprint8b(&r, 16, 0x123fff, "RESUME GAME");
//             // }
//             ggprint8b(&r, 16, 0xffffff, "NEW GAME");
//             ggprint8b(&r, 16, 0xffffff, "EXIT");
//             break;
//         default:
//             //            printf("FATAL GAME ERROR\n\n");
//             break;
//     }
// }
