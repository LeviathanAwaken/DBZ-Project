
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

class Global {
    public:
        GLuint lawrenceTexture;
} glob;
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
Image img[] = {"images/Goku.gif", "images/cloud.gif","images/lawrence.jpg"};

extern unsigned char *buildAlphaData(Image *);

void initialize()
{
    int w = imag[0].width;
    int h = imag[0].height;
    glGenTextures(1, &glob.lawrenceTexture);
    glBindTexture(GL_TEXTURE_2D, glob.lawrenceTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *walkData = buildAlphaData(&imag[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, walkData);
}

void showText(int x, int y)
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
    glColor3ub(255,255,255);
    static float angle = 0.0f;
    static int wid = 30;
    wid += sin(angle) * 10;
    float fx = (float)x;
    float fy = (float)y;
    float a = 0;
    a+= sin(angle) * 10.0f;
    fx += a;
    angle += 0.2f;
    glPushMatrix();
    glTranslate(fx,fy,0);
    glRotatef(a,0,0,1.0);
    glBindTexture(GL_TEXTURE_2D,glob.lawrenceTexture);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid,wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(wid,wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(wid,-wid);

    glEnd();
    glPopMatrix();

}
void showLawrence(int x, int y)
{
    initialize();
    showLawrencePicture(x+200, y);
    showText(x, y);
}

