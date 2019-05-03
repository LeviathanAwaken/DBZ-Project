//3350
//program: walk.cpp
//author:  Gordon Griesel
//date:    summer 2017 - 2018
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include "Global.h"
#include "Image.h"
#include "lawrenceM.h"
//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
				 (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
// const float timeslice = 1.0f;
// const float gravity = -0.2f;
#define ALPHA 1

//global declarations
int selectedOption = NEWGAME;
int gameState = MAINMENU;
int keys[65536];
int done = 0;

//CHANGED
//Tracks character's position

Image img[] = {"images/Goku.gif", "images/cloud.gif", "images/seanPic.gif",
	"images/joshPic.gif", "images/juanPic.gif", "images/Drakepic.gif",
	"images/lawrencePic.gif", "images/kiBlast.png", "images/namek.gif",
	"images/Saibaman.gif", "images/powerup.gif",
	"images/finalFormLogoTexture.gif","images/gordon1.png",
	"images/explosion.gif", "images/gokuss3.png", "images/gokussb.png",
	"images/explosion2.gif", "images/explosion3.gif", "images/bracket.png",
	"images/deathTexture.gif", "images/gokunorm.gif", "images/gokuss4.png",
	"images/gokurose.png", "images/blastPowerup.gif", "images/outlines.png"};



//-----------------------------------------------------------------------------
//Setup timers
class Timers {
	public:
		double physicsRate;
		double oobillion;
		struct timespec timeStart, timeEnd, timeCurrent;
		struct timespec walkTime;
		struct timespec menuSelectionDelay, menuSelectionTime;
		Timers() {
			physicsRate = 1.0 / 30.0;
			oobillion = 1.0 / 1e9;
		}
		double timeDiff(struct timespec *start, struct timespec *end) {
			return (double)(end->tv_sec - start->tv_sec ) +
			(double)(end->tv_nsec - start->tv_nsec) * oobillion;
		}
		void timeCopy(struct timespec *dest, struct timespec *source) {
			memcpy(dest, source, sizeof(struct timespec));
		}
		void recordTime(struct timespec *t) {
			clock_gettime(CLOCK_REALTIME, t);
		}
} timers;
//-----------------------------------------------------------------------------
Global g;

class X11_wrapper {
	private:
		Display *dpy;
		Window win;
	public:
		X11_wrapper() {
			GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER,
			None};
			//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
			XSetWindowAttributes swa;
			setupScreenRes(g.xres, g.yres);
			dpy = XOpenDisplay(NULL);
			if (dpy == NULL) {
				printf("\n\tcannot connect to X server\n\n");
				exit(EXIT_FAILURE);
			}
			Window root = DefaultRootWindow(dpy);
			XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
			if (vi == NULL) {
				printf("\n\tno appropriate visual found\n\n");
				exit(EXIT_FAILURE);
			}
			Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
			swa.colormap = cmap;
			swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
			StructureNotifyMask | SubstructureNotifyMask;
			win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
				vi->depth, InputOutput, vi->visual,
				CWColormap | CWEventMask, &swa);
			GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
			glXMakeCurrent(dpy, win, glc);
			setTitle();
		}
		~X11_wrapper() {
			XDestroyWindow(dpy, win);
			XCloseDisplay(dpy);
		}
		void setTitle() {
			//Set the window title bar.
			XMapWindow(dpy, win);
			XStoreName(dpy, win, "Walk Cycle");
		}
		void setupScreenRes(const int w, const int h) {
			g.xres = w;
			g.yres = h;
		}
		void reshapeWindow(int width, int height) {
			//window has been resized.
			setupScreenRes(width, height);
			glViewport(0, 0, (GLint)width, (GLint)height);
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
			glOrtho(0, g.xres, 0, g.yres, -1, 1);
			setTitle();
		}
		void checkResize(XEvent *e) {
			//The ConfigureNotify is sent by the
			//server if the window is resized.
			if (e->type != ConfigureNotify)
				return;
			XConfigureEvent xce = e->xconfigure;
			if (xce.width != g.xres || xce.height != g.yres) {
				//Window size did change.
			reshapeWindow(xce.width, xce.height);
			}
		}
		bool getXPending() {
			return XPending(dpy);
		}
		XEvent getXNextEvent() {
			XEvent e;
			XNextEvent(dpy, &e);
			return e;
		}
		void swapBuffers() {
			glXSwapBuffers(dpy, win);
		}
} x11;

//function prototypes
void initOpengl(void);
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void physics(void);
void render(void);

struct timespec tstart, tend;

int main(void)
{
	#ifdef PROFILE
		timers.recordTime(&tstart);
	#endif
	// asks for initials for scoreboard
		char p_name[3];
		printf("Enter 3 Initials to record score (letters only!): ");
		int pc = 0;
		while (pc < 3) {
			p_name[pc] = getchar();
			if (!isalpha(p_name[pc])) {
				printf("Enter only a letter!\n");
				p_name[pc] = getchar();
			}
			pc++;
		}

	extern int score_add2(char p_name[]);
		score_add2(p_name);
	initOpengl();
	init();
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.checkResize(&e);
			checkMouse(&e);
			done = checkKeys(&e);
		}
		physics();
		render();
		x11.swapBuffers();
		#ifdef PROFILE
			timers.recordTime(&tend);
		#endif
	}
	cleanup_fonts();
		// server side scores
		extern int score_show();
		extern int score_add(int);
		score_add(g.score);
		score_show();
	return 0;
}

unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void initOpengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//load the images file into a ppm structure.
	//
	int w = img[0].width;
	int h = img[0].height;
	//

	//create opengl texture elements
	glGenTextures(1, &g.walkTexture);   //Goku
	glGenTextures(1, &g.cloudTexture);  //Cloud

	//------------------------------Goku Texture--------------------------------
	//silhouette
	//this is similar to a sprite graphic
	//
	glBindTexture(GL_TEXTURE_2D, g.walkTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *walkData = buildAlphaData(&img[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//free(walkData);
	//unlink("./images/walk.ppm");
	//--------------------------------------------------------------------------

	//-----------------------Cloud Texture--------------------------------------
	w = img[1].width;
	h = img[1].height;

	glBindTexture(GL_TEXTURE_2D, g.cloudTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	walkData = buildAlphaData(&img[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//--------------------------Sean's Face-------------------------------------
	w = img[2].width;
	h = img[2].height;
	glGenTextures(1, &g.seanTexture);
	glBindTexture(GL_TEXTURE_2D, g.seanTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//---------------------------Josh Pic---------------------------------------
	w = img[3].width;
	h = img[3].height;
	glGenTextures(1, &g.joshTexture);
	glBindTexture(GL_TEXTURE_2D, g.joshTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------
	//------------------------Drake's Pic---------------------------------------
	w = img[5].width;
	h = img[5].height;
	glGenTextures(1, &g.drakeTexture);
	glBindTexture(GL_TEXTURE_2D, g.drakeTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//---------------------------Juan Pic---------------------------------------
	w = img[4].width;
	h = img[4].height;
	glGenTextures(1, &g.juanTexture);
	glBindTexture(GL_TEXTURE_2D, g.juanTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//---------------------Lawrence's Pic---------------------------------------
	w = img[6].width;
	h = img[6].height;
	glGenTextures(1, &g.lawrenceTexture);
	glBindTexture(GL_TEXTURE_2D, g.lawrenceTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[6]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//--------------------------KiBlast Texture---------------------------------
	w = img[7].width;
	h = img[7].height;
	glGenTextures(1, &g.kiTexture);
	glBindTexture(GL_TEXTURE_2D, g.kiTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[7]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//--------------------------saibaman Texture---------------------------------
	w = img[9].width;
	h = img[9].height;
	glGenTextures(1, &g.saibaTexture);
	glBindTexture(GL_TEXTURE_2D, g.saibaTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[9]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//------------------------Namek Background----------------------------------
	w = img[8].width;
	h = img[8].height;
	glGenTextures(1, &g.namekTexture);
	glBindTexture(GL_TEXTURE_2D, g.namekTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[8]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
//--------------------------------------------------------------------------

//-------------------------Power-Up Texture---------------------------------
	w = img[10].width;
	h = img[10].height;
	glGenTextures(1, &g.powerupTexture);
	glBindTexture(GL_TEXTURE_2D, g.powerupTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[10]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//-------------------------LogoTexture---------------------------------
	w = img[11].width;
	h = img[11].height;
	glGenTextures(1, &g.finalFormLogoTexture);
	glBindTexture(GL_TEXTURE_2D, g.finalFormLogoTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[11]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//------------------------Boss----------------------------------
	w = img[12].width;
	h = img[12].height;
	glGenTextures(1, &g.bossTexture);
	glBindTexture(GL_TEXTURE_2D, g.bossTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[12]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------


	//------------------------explosion----------------------------------
	w = img[13].width;
	h = img[13].height;
	glGenTextures(1, &g.explosionTexture);
	glBindTexture(GL_TEXTURE_2D, g.explosionTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[13]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//------------------------Goku SS3----------------------------------
	w = img[14].width;
	h = img[14].height;
	glGenTextures(1, &g.ss3Texture);
	glBindTexture(GL_TEXTURE_2D, g.ss3Texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[14]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------


	//------------------------Goku SSB----------------------------------
	w = img[15].width;
	h = img[15].height;
	glGenTextures(1, &g.ssbTexture);
	glBindTexture(GL_TEXTURE_2D, g.ssbTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[15]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//------------------------Explosion 2----------------------------------
	w = img[16].width;
	h = img[16].height;
	glGenTextures(1, &g.explosion2Texture);
	glBindTexture(GL_TEXTURE_2D, g.explosion2Texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[16]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//------------------------Explosion 2----------------------------------
	w = img[17].width;
	h = img[17].height;
	glGenTextures(1, &g.explosion3Texture);
	glBindTexture(GL_TEXTURE_2D, g.explosion3Texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[17]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//------------------------Explosion 2----------------------------------
	w = img[18].width;
	h = img[18].height;
	glGenTextures(1, &g.braceTexture);
	glBindTexture(GL_TEXTURE_2D, g.braceTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[18]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//--------------------------------------------------------------------------

	//------------------------death texture----------------------------------
	w = img[19].width;
	h = img[19].height;
	glGenTextures(1, &g.deathTexture);
	glBindTexture(GL_TEXTURE_2D, g.deathTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[19]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, walkData);

	//--------------------------------------------------------------------------

	//------------------------Norm Form texture---------------------------------
	w = img[20].width;
	h = img[20].height;
	glGenTextures(1, &g.normTexture);
	glBindTexture(GL_TEXTURE_2D, g.normTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[20]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, walkData);

	//--------------------------------------------------------------------------

	//------------------------SS4 texture----------------------------------
	w = img[21].width;
	h = img[21].height;
	glGenTextures(1, &g.ss4Texture);
	glBindTexture(GL_TEXTURE_2D, g.ss4Texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[21]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, walkData);

	//--------------------------------------------------------------------------

	//------------------------SS Rose texture----------------------------------
	w = img[22].width;
	h = img[22].height;
	glGenTextures(1, &g.ssrTexture);
	glBindTexture(GL_TEXTURE_2D, g.ssrTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[22]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, walkData);

	//--------------------------------------------------------------------------

	//----------------------BlastPowerup texture--------------------------------
	w = img[23].width;
	h = img[23].height;
	glGenTextures(1, &g.blastPowerupTexture);
	glBindTexture(GL_TEXTURE_2D, g.blastPowerupTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[23]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	 //--------------------------------------------------------------------------

	//----------------------BlastPowerup texture--------------------------------
	w = img[24].width;
	h = img[24].height;
	glGenTextures(1, &g.outlineTexture);
	glBindTexture(GL_TEXTURE_2D, g.outlineTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	walkData = buildAlphaData(&img[24]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	 //--------------------------------------------------------------------------

}

extern void sInit(GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
extern void Enemy_init();
extern void Powerups_init();
extern void blastPowerup_init();
void init()
{
	//CHANGED - initializes character's position and velocity
	sInit(g.normTexture, g.walkTexture, g.ss3Texture,
		g.ss4Texture, g.ssrTexture, g.ssbTexture);
	Enemy_init();
	Powerups_init();
	blastPowerup_init();
	img[13].rows = 9;
	img[13].columns = 9;
	img[16].rows = 6;
	img[16].columns = 5;
	img[17].rows = 9;
	img[17].columns = 8;
	img[24].rows = 1;
	img[24].columns = 4;
}
// extern void checkMouseMenu(XEvent*);

void checkMouse(XEvent *e)
{
	switch(gameState) {
		case MAINMENU:
			// checkMouseMenu(e);
			break;
		case PAUSEMENU:
			break;
		case DEATH:
			break;
		case INGAME: {
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
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
	}
}

extern void launchKi();

int checkKeys(XEvent *e)
{

	//keyboard input?
	// static int shift=0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	//Log("key: %i\n", key);
	if (e->type == KeyRelease)
	{
		g.keys[key] = 0;
		// if (key == XK_Shift_L || key == XK_Shift_R)
		// 	shift=0;
	}
	if (e->type == KeyPress)
	{
		//std::cout << "press" << std::endl;
		g.keys[key] = 1;
		// if (key == XK_Shift_L || key == XK_Shift_R) {
		// 	shift=1;
	}

	/*if (e->type != KeyRelease && e->type != KeyPress)
	  return 0;
	  int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	  if (e->type == KeyRelease) {
	  g.keys[key] = 0;
	  if (key == XK_Shift_L || key == XK_Shift_R)
	  shift = 0;
	  return 0;
	  }
	  if (key == XK_Shift_L || key == XK_Shift_R) {
	  shift=1;
	  return 0;
	  }

	  if ( e->type == KeyPress) {
	  g.keys[key] = 1;
	  if (key == XK_Shift_L || XK_Shift_R) {
	  shift = 1;
	  return 0;
	  } else {
	  return 0;
	  }
	  */

	// (void)shift;
	//CHANGED - updates velocity with the listed keys
	//CHANGED - modified movement to get rid of delay on keypress
	//CHANGED - moved goku's movement keypress handler statements into physics
	switch (gameState)
	{
	case MAINMENU:
		break;
	case PAUSEMENU:
		break;
	case DEATH:
		break;
	case INGAME:
		g.delay = 0.01; // Sets speed to max at start of game
		switch (key)
		{
		// case XK_c:
		// 	g.creditFlag ^= 1;
		// 	timers.recordTime(&timers.walkTime);
		// 	g.walk ^= 1;
		// 	break;
		case XK_space:
			timers.recordTime(&timers.walkTime);
			g.walk ^= 1;
			break;
		case XK_k:
			launchKi();
			break;
			/*        case XK_equal:
			  g.delay -= 0.005;
			  if (g.delay < 0.005)
			  g.delay = 0.005;
			  break;
			  case XK_minus:
			  g.delay += 0.005;
			  break;
			  */
		case XK_Escape:
			//extern void score_get();
			//score_get();
			gameState = PAUSEMENU;
			selectedOption = RESUMEGAME;
			break;
		//case XK_j:
		//	g.score++;
		//	break;
		// case XK_z:
		// 	// if (g.paused == true && g.startFlag == 0)
		// 	{
		// 		g.startFlag ^= 1;
		// 		g.paused = !g.paused;
		// 	}
		// 	break;
		// case XK_p:
		// 	if (g.startFlag == 1)
		// 	{
		// 		g.pauseFlag ^= 1;
		// 		g.paused = !g.paused;
		// 	}
		// 	break;

		default:
			break;
		}
		return 0;
		break;
	default:
		printf("FATAL ERROR IN GAME STATE\n\n");
		exit(1);
	}


	return 0;
}

Flt VecNormalize(Vec vec)
{
	Flt len, tlen;
	Flt xlen = vec[0];
	Flt ylen = vec[1];
	Flt zlen = vec[2];
	len = xlen * xlen + ylen * ylen + zlen * zlen;
	if (len == 0.0)
	{
		MakeVector(0.0, 0.0, 1.0, vec);
		return 1.0;
	}
	len = sqrt(len);
	tlen = 1.0 / len;
	vec[0] = xlen * tlen;
	vec[1] = ylen * tlen;
	vec[2] = zlen * tlen;
	return (len);
}

extern void kiHandler(int);
extern void saibaPhysics();
extern void bossPhysics();
extern void powerupsPhysics();
extern void blastPowerupPhysics();
//extern void velUpd(int);
//extern void gokuMove();
extern void gokuIMove(int);
extern void checkKeysMainMenu();
extern void checkKeysPauseMenu();
extern void checkKeysLost();
extern void braceHandler(int);
extern void namekPhysics();

void physics(void)
{
	switch (gameState)
	{
	case MAINMENU:
		checkKeysMainMenu();
		break;
	case PAUSEMENU:
		checkKeysPauseMenu();
		break;
	case DEATH:
		checkKeysLost();
		break;
	case INGAME:
		if (g.walk) {
			//man is walking...
			//when time is up, advance the frame.
			timers.recordTime(&timers.timeCurrent);
			double timeSpan = timers.timeDiff(&timers.walkTime,
							&timers.timeCurrent);
			if (timeSpan > g.delay) {
				//advance
				//CHANGED - shifts goku's pos by velocity, resets velocity
				//          if character hits window edges
				//++g.walkFrame;
				//gokuMove();
				if (g.walkFrame >= 16)
					g.walkFrame -= 16;
				timers.recordTime(&timers.walkTime);
				kiHandler(0);
				braceHandler(0);
			}
			for (int i=0; i<20; i++) {
				g.box[i][0] -= 0.3 * (0.05 / g.delay);
				if (g.box[i][0] < -10.0)
					g.box[i][0] += g.xres + 10.0;
			}
			saibaPhysics();
			bossPhysics();
			powerupsPhysics();
			blastPowerupPhysics();
			namekPhysics();


			//------------------check for movement keys-----------------------------
				if (g.keys[XK_a] || g.keys[XK_Left]) {
					gokuIMove(0);
				}
				if (g.keys[XK_d] || g.keys[XK_Right]) {
					gokuIMove(1);
				}
				if (g.keys[XK_w] || g.keys[XK_Up]) {
					gokuIMove(2);
				}
				if (g.keys[XK_s] || g.keys[XK_Down]) {
					gokuIMove(3);
				}
			}
		}
	}

extern void showSean(int, int, GLuint);
extern void showJoshua(int, int, GLuint);
extern void showDrake(int, int, GLuint);
extern void showJuan(int, int, GLuint);
extern void showLawrence(int,int,GLuint);
extern void enemyHandler(GLuint, GLuint);
extern void setBackgroundNamek(int, int, GLuint);
extern void powerupsRender(GLuint);
extern void sRender();
extern void renderMainMenuBackground(int , int, GLuint);
extern void renderMainMenu();
extern void renderPauseMenu();
extern void explosionRender();
extern void cleanExplosions();
extern void renderDeath();
extern void renderControls();
extern void blastPowerupRender(GLuint);
extern void renderHealthBar();

void render(void)
{
	switch(gameState)
	{
		case MAINMENU:
			renderMainMenuBackground(g.xres, g.yres, g.namekTexture);
			if (g.controlFlag == 1) {
				renderControls();
			}
			renderMainMenu();
			break;
		case PAUSEMENU:
			if (g.controlFlag == 1) {
				renderControls();
			}
			renderPauseMenu();
			break;
		case DEATH:
			if(g.controlFlag == 1) {
				renderControls();
			}
			renderDeath();
		//need to develop death screen
			break;
		case INGAME: {
		//Put picture functions here
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		showSean(20, img[2].height, g.seanTexture);
		showLawrence(40, img[6].height,g.lawrenceTexture);
		showJoshua(40, img[3].height, g.joshTexture);
		showDrake(70, img[5].height, g.drakeTexture);
		showJuan(40, img[4].height, g.juanTexture);

		// if (g.pauseFlag) {
		// 	extern void showPause(int, int);
		// 	showPause(350, 100);

			// Rect r;
			//Clear the screen
			glClearColor(0.1, 0.1, 0.1, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			//show ground
			setBackgroundNamek(g.xres, g.yres/*img[7].height*/, g.namekTexture);
			glEnd();
			//
			//fake shadow
			//glColor3f(0.25, 0.25, 0.25);
			//glBegin(GL_QUADS);
			//  glVertex2i(cx-60, 150);
			//  glVertex2i(cx+50, 150);
			//  glVertex2i(cx+50, 130);
			//  glVertex2i(cx-60, 130);
			//glEnd();
			//
			//show boxes as background
			for (int i=0; i<20; i++) {
				glPushMatrix();
				glTranslated(g.box[i][0],g.box[i][1],g.box[i][2]);
				glColor3f(1.0, 1.0, 1.0);
				glBindTexture(GL_TEXTURE_2D, g.cloudTexture);
				//
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.0f);
				glColor4ub(255,255,255,255);

				float tx = 0, ty = 0;

				// Render Clouds
				glBegin(GL_QUADS);
					glTexCoord2f(tx+1,      ty+1); glVertex2i(0, 0);
					glTexCoord2f(tx+1,      ty);    glVertex2i(0, 30);
					glTexCoord2f(tx, ty);    glVertex2i(40, 30);
					glTexCoord2f(tx, ty+1); glVertex2i(40, 0);
				glEnd();

				glPopMatrix();

				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_ALPHA_TEST);
			}

			enemyHandler(g.saibaTexture, g.bossTexture);

			powerupsRender(g.powerupTexture);

			blastPowerupRender(g.blastPowerupTexture);

			explosionRender();
			cleanExplosions();
			sRender();

			//
			// unsigned int c = 0x000000;
			// r.bot = g.yres - 20;
			// r.left = 10;
			// r.center = 0;

			// ggprint8b(&r, 16, c, "K   Ki Blast");
			// ggprint8b(&r, 16, c, "right arrow/d -> fly right");
			// ggprint8b(&r, 16, c, "left arrow/a  <- fly left");
			// ggprint8b(&r, 16, c, "up arrow/w -> fly up");
			// ggprint8b(&r, 16, c, "down arrow/s -> fly down");
			// ggprint8b(&r, 16, c, "j -> test temp score update");
			// ggprint8b(&r, 16, c, "p -> test pause screen");
			// ggprint8b(&r, 16, c, "frame: %i", g.walkFrame);
			extern void showScore(int, int, int);
			extern int score_receive();
			g.score += score_receive();
			showScore(5, 22, g.score);
			// if (g.startFlag == 0) {
			// 	extern void showStart(int, int);
			// 	showStart(g.xres/3, g.yres/7);
			// }
			if (g.controlFlag == 1) {
				renderControls();
			}
			extern void showTimes(int, int, double);
			showTimes(g.xres/5, -15, timers.timeDiff(&tstart, &tend));
			extern void gokuHealth(int, int);
			gokuHealth(g.xres/5, -15);
			renderHealthBar();
	}
	}
}
