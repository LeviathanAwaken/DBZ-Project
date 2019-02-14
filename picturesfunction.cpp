
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
    glBindTexture(GL_TEXTURE_2D,textid);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid,wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(wid,wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(wid,-wid);

    glEnd();
    glPopMatrix();



}

