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
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include "Explosion.h"
#include "Global.h"


/* trying to figure out how to add power ups as well as explosions
std::vector<PowerUp> powerUps;
std::vector<Explosion> explosions;
*/

//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1

//CHANGED
//Tracks character's position

class Protag {
public:
    Vec pos;
    Vec vel;
} goku;


class Image {
    public:
        int width, height;
        unsigned char *data;
        ~Image() { delete [] data; }
        Image(const char *fname) {
            if (fname[0] == '\0')
                return;
            //printf("fname **%s**\n", fname);
            char name[40];
            strcpy(name, fname);
            int slen = strlen(name);
            name[slen-4] = '\0';
            //printf("name **%s**\n", name);
            char ppmname[80];
            sprintf(ppmname,"%s.ppm", name);
            //printf("ppmname **%s**\n", ppmname);
            char ts[100];
            //system("convert eball.jpg eball.ppm");
            sprintf(ts, "convert %s %s", fname, ppmname);
            system(ts);
            //sprintf(ts, "%s", name);
            FILE *fpi = fopen(ppmname, "r");
            if (fpi) {
                char line[200];
                fgets(line, 200, fpi);
                fgets(line, 200, fpi);
                while (line[0] == '#')
                    fgets(line, 200, fpi);
                sscanf(line, "%i %i", &width, &height);
                fgets(line, 200, fpi);
                //get pixel data
                int n = width * height * 3;
                data = new unsigned char[n];
                for (int i=0; i<n; i++)
                    data[i] = fgetc(fpi);
                fclose(fpi);
            } else {
                printf("ERROR opening image: %s\n",ppmname);
                exit(0);
            }
            unlink(ppmname);
        }
};

Image img[] = {"images/Goku.gif", "images/cloud.gif", "images/seanPic.gif",
    "images/joshPic.gif", "images/juanPic.gif", "images/Drakepic.gif",
    "images/lawrencePic.gif", "images/kiBlast.png", "images/enemy.gif","images/kiBlast.png", "images/namek.gif"};

//-----------------------------------------------------------------------------
//Setup timers
class Timers {
    public:
        double physicsRate;
        double oobillion;
        struct timespec timeStart, timeEnd, timeCurrent;
        struct timespec walkTime;
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



class X11_wrapper {
    private:
        Display *dpy;
        Window win;
    public:
        X11_wrapper() {
            GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER,
                    None };
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
            XStoreName(dpy, win, "DBZ: The Final Form");
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


int main(void)
{
    initOpengl();
    init();
    int done = 0;
    while (!done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.checkResize(&e);
            checkMouse(&e);
            done = checkKeys(&e);
        }
    if (g.paused == false || g.startFlag == 0)
            physics();
        render();
        x11.swapBuffers();
    }
    cleanup_fonts();
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

    //--------------------------Enemy Texture---------------------------------
    w = img[9].width;
    h = img[9].height;
    glGenTextures(1, &g.enemyTexture);
    glBindTexture(GL_TEXTURE_2D, g.enemyTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    walkData = buildAlphaData(&img[9]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, walkData);
    //--------------------------------------------------------------------------

    //--------------------------Title Screen Texture---------------------------------
    w = img[10].width;
    h = img[10].height;
    glGenTextures(1, &g.titleTexture);
    glBindTexture(GL_TEXTURE_2D, g.titleTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    walkData = buildAlphaData(&img[]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, walkData);
    //--------------------------------------------------------------------------
}

extern void sInit(GLuint, int, int);

void init()
{
    //CHANGED - initializes character's position and velocity
    MakeVector(0.0, 100.0, 0.0, goku.pos);
    VecZero(goku.vel);
    sInit(g.kiTexture, g.xres, g.yres);
}

void checkMouse(XEvent *e)
{
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

extern void launchKi(int, int);

int checkKeys(XEvent *e)
{
    //keyboard input?
    static int shift=0;
    int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
    //Log("key: %i\n", key);
    if (e->type == KeyRelease) {
        g.keys[key]=0;
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift=0;
        return 0;
    }
    if (e->type == KeyPress) {
        //std::cout << "press" << std::endl;
        g.keys[key]=1;
        if (key == XK_Shift_L || key == XK_Shift_R) {
            shift=1;
            return 0;
        }
    } else {
        return 0;
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

    (void)shift;
    //CHANGED - updates velocity with the listed keys
    //CHANGED - modified movement to get rid of delay on keypress
    //CHANGED - moved goku's movement keypress handler statements into physics
    switch (key) {
        case XK_c:
            g.creditFlag ^= 1;
        // [[fallthrough]];
        case XK_space:
            timers.recordTime(&timers.walkTime);
            g.walk ^= 1;
            break;
        case XK_k:
            launchKi(goku.pos[0] + 50, goku.pos[1]);
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
            return 1;
            break;
    case XK_j:
        g.score++;
        break;
        case XK_z:
        if (g.paused == true && g.startFlag == 0) {
            g.startFlag ^= 1;
        g.paused = !g.paused;
        }
            //
            break;
        case XK_p:
        if (g.startFlag == 1) {
                g.pauseFlag ^= 1;
            g.paused = !g.paused;
        }
            break;

    }
    g.delay = 0.005;    // Sets speed to max at start of game

    return 0;
}

Flt VecNormalize(Vec vec)
{
    Flt len, tlen;
    Flt xlen = vec[0];
    Flt ylen = vec[1];
    Flt zlen = vec[2];
    len = xlen*xlen + ylen*ylen + zlen*zlen;
    if (len == 0.0) {
        MakeVector(0.0,0.0,1.0,vec);
        return 1.0;
    }
    len = sqrt(len);
    tlen = 1.0 / len;
    vec[0] = xlen * tlen;
    vec[1] = ylen * tlen;
    vec[2] = zlen * tlen;
    return(len);
}

extern void kiHandler(int);

void physics(void)
{
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
            if ((goku.pos[0] > (- g.xres / 2 + 50) && goku.vel[0] < 0)
                || (goku.pos[0] < (g.xres / 2 - 50) && goku.vel[0] > 0))
                goku.pos[0] += goku.vel[0];
            else
                goku.vel[0] = 0;
            if ((goku.pos[1] > (-g.yres / 2 + 50) && goku.vel[1] < 0)
                || (goku.pos[1] < (g.yres / 2 - 50) && goku.vel[1] > 0))
                goku.pos[1] += goku.vel[1];
            else
                goku.vel[1] = 0;
            if (g.walkFrame >= 16)
                g.walkFrame -= 16;
            timers.recordTime(&timers.walkTime);
            kiHandler(0);
        }
        for (int i=0; i<20; i++) {
            g.box[i][0] -= 2.0 * (0.05 / g.delay);
            if (g.box[i][0] < -10.0)
                g.box[i][0] += g.xres + 10.0;
        }

        //------------------check for movement keys-----------------------------
        if (g.startFlag == 1) {
        if (g.keys[XK_a] || g.keys[XK_Left]) {
                goku.vel[0]--;
            }
            if (g.keys[XK_d] || g.keys[XK_Right]) {
                goku.vel[0]++;
            }
            if (g.keys[XK_w] || g.keys[XK_Up]) {
                goku.vel[1]++;
            }
            if (g.keys[XK_s] || g.keys[XK_Down]) {
                goku.vel[1]--;
            }
    }
    }
}

extern void showSean(int, int, GLuint);
extern void showJoshua(int, int, GLuint);
extern void showDrake(int, int, GLuint);
extern void showJuan(int, int, GLuint);
extern void showLawrence(int,int,GLuint);
extern void setBackgroundNamek(int, int, GLuint);

void render(void)
{
    if (g.creditFlag && !g.pauseFlag) {
        //Put picture functions here
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        showSean(20, img[2].height, g.seanTexture);
        showLawrence(40, img[6].height,g.lawrenceTexture);
        showJoshua(40, img[3].height, g.joshTexture);
        showDrake(70, img[5].height, g.drakeTexture);
        showJuan(40, img[4].height, g.juanTexture);

    } else if (g.pauseFlag) {
        extern void showPause(int, int);
        showPause(350, 100);
    } else {
        Rect r;
        //Clear the screen
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        

        float cx = g.xres/2.0;
        float cy = g.yres/2.0;
        //
        //show ground
        setBackgroundNamek(0, img[7].height, g.namekTexture);
        glEnd();
        //using to draw powerups but not implemented yet at all
        // cleanPowerUps();
        // for(unsigned int i = 0; i < powerUps.size(); i++){
        //     powerUps[i].draw();
        // }
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

        // CHANGED
        // THIS IS THE CHARACTERS SIZE
        float h = 50.0;
        float w = h * 1;
        glPushMatrix();
        //CHANGED - Moves the Character
        glTranslatef(goku.pos[0], goku.pos[1], goku.pos[2]);
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, g.walkTexture);
        //
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);

        // CHANGED
        int ix = 0; //g.walkFrame % 8;
        int iy = 0;
        if (g.walkFrame >= 8)
            iy = 1;

        //CHANGED
        float tx = (float)ix; // / 8.0;
        float ty = (float)iy; // / 2.0;

        glBegin(GL_QUADS);
        glTexCoord2f(tx+1,      ty+1); glVertex2i(cx-w, cy-h);
        glTexCoord2f(tx+1,      ty);    glVertex2i(cx-w, cy+h);
        glTexCoord2f(tx, ty);    glVertex2i(cx+w, cy+h);
        glTexCoord2f(tx, ty+1); glVertex2i(cx+w, cy-h);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        kiHandler(1);
        //
        unsigned int c = 0x000000;
        r.bot = g.yres - 20;
        r.left = 10;
        r.center = 0;
        ggprint8b(&r, 16, c, "Spacebar   Walk cycle");
        ggprint8b(&r, 16, c, "+   faster");
        ggprint8b(&r, 16, c, "-   slower");
        ggprint8b(&r, 16, c, "right arrow/d -> fly right");
        ggprint8b(&r, 16, c, "left arrow/a  <- fly left");
        ggprint8b(&r, 16, c, "up arrow/w -> fly up");
        ggprint8b(&r, 16, c, "down arrow/s -> fly down");
        ggprint8b(&r, 16, c, "j -> test temp score update");
        ggprint8b(&r, 16, c, "p -> test pause screen");
        ggprint8b(&r, 16, c, "frame: %i", g.walkFrame);
        extern void showScore(int, int, int);
        showScore(5, 22, g.score);
        if (g.startFlag == 0) {
            extern void showStart(int, int);
            showStart(330, 100);
        }
    }
}
