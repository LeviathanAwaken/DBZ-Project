// Created By: Juan Trigueros
// Date: 02-14-19
//
//Shows credit pic, shows temporary start/pause screens and score keeper

#include <GL/glx.h>
#include "fonts.h"
#include <ctime>
#include <cstdio>
#include <cstring>

#ifdef PROFILE
//This code is activated only for profiling while optimization
//is attempted.
const double oobillion = 1.0/1e9; //one over a billion.
struct timespec timeStart, timeCurrent;
double total_0 = 0.0;
double total_1 = 0.0;
double total_2 = 0.0;
double total_3 = 0.0;
double total_4 = 0.0;
void timeGet(struct timespec *t)
{
    clock_gettime(CLOCK_REALTIME, t);
}
double timeDiff(struct timespec *start, struct timespec *end)
{
    //return the difference in two times.
    double ret =
        (double)(end->tv_sec - start->tv_sec ) +
        (double)(end->tv_nsec - start->tv_nsec) * oobillion;
    return ret;
}
//void timeCopy(struct timespec *dest, struct timespec *source)
//{
    //copy one time structure to another.
  //  memcpy(dest, source, sizeof(struct timespec));
//}
#endif


void showJuanPic(int x, int y, GLuint textid)
{
    glBindTexture(GL_TEXTURE_2D, textid);
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,1.0f);glVertex2i(x+450,y+150);
        glTexCoord2f(0.0f,0.0f);glVertex2i(x+450,y+300);
        glTexCoord2f(1.0f,0.0f);glVertex2i(x+675,y+300);
        glTexCoord2f(1.0f,1.0f);glVertex2i(x+675,y+150);
    glEnd();
    glPopMatrix();
}

void showJuanText(int x, int y)
{
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y-20;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 22, c, "Juan Trigueros");
}

void showScore(int x, int y, int z)
{
    #ifdef PROFILE
        struct timespec tstart, tend;
        timeGet(&tstart);
    #endif
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y-20;
    r.left = x;
    r.center = 0;
    ggprint16(&r, 22, c,"Score: %d", z);
    #ifdef PROFILE
        timeGet(&tend);
        total_0 += timeDiff(&tstart, &tend);
    #endif
}

void showJuan(int x, int y, GLuint textInt)
{
    showJuanPic(x-400,y-950,textInt);
    showJuanText(x+125,y-800);
}

void showStart(int x, int y)
{
    #ifdef PROFILE
        struct timespec tstart, tend;
        timeGet(&tstart);
    #endif

    Rect r;
    unsigned int c = 0x7c0095;
    r.bot = y-20;
    r.left = x;
    r.center = 0;
    ggprint16(&r, 16, c,"press z to start");
    #ifdef PROFILE
        timeGet(&tend);
        total_1 += timeDiff(&tstart, &tend);
    #endif

}

void showPause(int x, int y)
{
    #ifdef PROFILE
        struct timespec tstart, tend;
        timeGet(&tstart);
    #endif
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y-20;
    r.left = x;
    r.center = 0;
    ggprint16(&r, 22, c,"paused");
    #ifdef PROFILE
        timeGet(&tend);
        total_2 += timeDiff(&tstart, &tend);
    #endif
}

#ifdef PROFILE
void showTimes(int x, int y, double runtime) {
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y+20;
    r.left = x+20;
    r.center = 0;
    ggprint16(&r, 16, c,"Run time: %d seconds", (int)runtime);
}
#endif
