// Created By: Juan Trigueros
// Date: 02-14-19
//
//Shows credit pic, shows temporary start/pause screens and score keeper

#include <GL/glx.h>
#include "fonts.h"
#include <ctime>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
using namespace std;

int score_glob = 0;

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
    double ret = (double)(end->tv_sec - start->tv_sec ) +
        (double)(end->tv_nsec - start->tv_nsec) * oobillion;
    return ret;
}
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
    showJuanPic(x-450,y-170,textInt);
    showJuanText(x+70,y+160);
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
void showTimes(int x, int y, double runtime) 
{
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y+20;
    r.left = x+20;
    r.center = 0;
    ggprint16(&r, 16, c,"Run time: %d seconds", (int)runtime);
}
#endif

#define PORT 443
#define USERAGENT "CMPS-3350"

const int MAX_READ_ERRORS = 100;

BIO *ssl_setup_bio(void);
void set_non_blocking(const int sock);
void get_a_page(SSL *ssl, char *hostname, char *pagename);
void get_a_page2(SSL *ssl, char *hostname, char *pagename);

int score_add (int score)
{
    char scor[1000];
    sprintf(scor, "%d", score);
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;
    BIO *outbio = NULL;

    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    char hostname[256] = "odin.cs.csub.edu";
    char pagename[256] = "/~jtrigueros/3350/project/scores.php?test=";
    strcat(pagename, scor);
    int port = PORT;

    //Setup the SSL BIO
    outbio = ssl_setup_bio();
    //Initialize the SSL library
    if (SSL_library_init() < 0)
        BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

    //Setup the socket used for connection.
    host = gethostbyname(hostname);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        BIO_printf(outbio, "Cannot connect to host %s [%s] on port %d.\n",
                hostname, inet_ntoa(addr.sin_addr), port);
    }
    //Connect using the SSL certificate.
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sd);
    SSL_connect(ssl);
    //A non-blocking socket will make the ssl_read() not block.
    set_non_blocking(sd);

    get_a_page(ssl, hostname, pagename);

    //Cleanup.
    SSL_free(ssl);
    close(sd);
    SSL_CTX_free(ctx);

    return 0;
}

void score_get ()
{
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;
    BIO *outbio = NULL;

    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    char hostname[256] = "odin.cs.csub.edu";
    char pagename[256] = "/~jtrigueros/3350/project/scores.html";
    int port = PORT;

    //Setup the SSL BIO
    outbio = ssl_setup_bio();
    //Initialize the SSL library
    if (SSL_library_init() < 0)
        BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

    //Setup the socket used for connection.
    host = gethostbyname(hostname);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        BIO_printf(outbio, "Cannot connect to host %s [%s] on port %d.\n",
                hostname, inet_ntoa(addr.sin_addr), port);
    }
    //Connect using the SSL certificate.
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sd);
    SSL_connect(ssl);
 //A non-blocking socket will make the ssl_read() not block.
    set_non_blocking(sd);

    get_a_page(ssl, hostname, pagename);

    //Cleanup.
    SSL_free(ssl);
    close(sd);
    SSL_CTX_free(ctx);
}

int score_show ()
{
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;
    BIO *outbio = NULL;
    //Fixed Warning: Invalid conversion from const SSL_METHOD to SSL_METHOD
    //SSL_METHOD *method;
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    //char req[1000];
    //int req_len;
    char hostname[256] = "odin.cs.csub.edu";
    char pagename[256] = "/~jtrigueros/3350/project/scores.txt";
    int port = PORT;
    //Setup the SSL BIO
    outbio = ssl_setup_bio();
    //Initialize the SSL library
    if (SSL_library_init() < 0)
        BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);
    //next 2 lines of code are not currently needed.
    //SSL_MODE_AUTO_RETRY flag of the SSL_CTX_set_mode call.
    //SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);
    //
    //Setup the socket used for connection.
    host = gethostbyname(hostname);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        BIO_printf(outbio, "Cannot connect to host %s [%s] on port %d.\n",
  hostname, inet_ntoa(addr.sin_addr), port);
    }
    //Connect using the SSL certificate.
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sd);
    SSL_connect(ssl);
    //
    //A non-blocking socket will make the ssl_read() not block.
    set_non_blocking(sd);

    get_a_page2(ssl, hostname, pagename);

    //Cleanup.
    SSL_free(ssl);
    close(sd);
    SSL_CTX_free(ctx);

    return 0;
}


void get_a_page(SSL *ssl, char *hostname, char *pagename)
{
    char req[1000];
    int req_len;
    int ret;
    sprintf(req,"GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n",
            pagename, hostname, USERAGENT);
    req_len = strlen(req);
    ret = SSL_write(ssl, req, req_len);
    if (ret <= 0) {
    	fprintf(stderr, "ERROR: SSL_write\n");fflush(stderr);
    }
    if (ret <= 0) {
        fprintf(stderr, "ERROR: SSL_write\n");
        fflush(stderr);
    }
}

void get_a_page2(SSL *ssl, char *hostname, char *pagename)
{
    char req[1000];
    int req_len;
    int bytes, nreads, nerrs;
    char buf[3000];
    int ret;
    //Send the http request to the host server.
    sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n",
            pagename, hostname, USERAGENT);
    req_len = strlen(req);
    ret = SSL_write(ssl, req, req_len);
    //Fixed Warning: if statement not guarding
    if (ret <= 0) {
        fprintf(stderr, "ERROR: SSL_write\n");fflush(stderr);
    }
    if (ret <= 0) {
        fprintf(stderr, "ERROR: SSL_write\n");
        fflush(stderr);
    }

    //Get data returned from the server.
    //First, do priming read.
    //We can take this approach because our socket is non-blocking.
    //Start with an error condition.
    bytes = -1;
    memset(buf, '\0', sizeof(buf));
    while (bytes <= 0) {
        bytes = SSL_read(ssl, buf, sizeof(buf));
        //A slight pause can cause fewer reads to be needed.
        usleep(10000);
    }
    //A successful priming read was accomplished.
    //Now read all the data.
    nreads = 1;
    //Allow for some read errors to happen, while getting the complete data.
    nerrs = 0;
    int scrs[10000];
    for (int i = 0; i < 10000; i++)
        scrs[i] = -222;
    int ij = 0;
    int kl = 0;
    printf ("\nAll scores recorded to server (your score is the bottom most recent score): ");
    while (buf[ij] != '\0') {
        if (buf[ij] == '%') {
            ij++;
            break;
        }
        ij++;
    }
    while (buf[ij] != '\0') {
        if(isdigit(buf[ij])) {
            scrs[kl] = buf[ij] - '0';
            printf("%d",scrs[kl]);
            kl++;
        } else if (buf[ij] == '\n') {
            scrs[kl] = -111;
            printf("\n");
            kl++;
        }
        ij++;
    }
    //check from outside the above while loop
    while (bytes >= 0 && nerrs < MAX_READ_ERRORS) {
        //write(STDOUT_FILENO, buf, bytes);
        memset(buf,'\0', sizeof(buf));
        ++nreads;
        bytes = SSL_read(ssl, buf, sizeof(buf));
        if (bytes == 0)
            ++nerrs;
        else
            nerrs = 0;
        //A slight pause can cause fewer reads to be needed.
        usleep(20000);
    }
}

BIO *ssl_setup_bio(void)
{
    BIO *bio = NULL;
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();
    bio = BIO_new(BIO_s_file());
    bio = BIO_new_fp(stdout, BIO_NOCLOSE);
    return bio;
}

void set_non_blocking (const int sock)
{
    //Set a socket to be non-blocking.
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0) {
        perror("ERROR: fcntl(F_GETFL)");
        exit(EXIT_FAILURE);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock, F_SETFL, opts) < 0) {
        perror("ERROR: fcntl(O_NONBLOCK)");
        exit(EXIT_FAILURE);
    }
}

void score_update (int score)
{
    score_glob += score;
}

int score_receive () {
    int score_tmp;
    score_tmp = score_glob;
    score_glob = 0;
    return score_tmp;
}

int score_reset ()
{
    score_glob = 0;
    return 0;
}
