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

// #include <openssl/crypto.h>
// #include <openssl/x509.h>
// #include <openssl/pem.h>
// #include <openssl/ssl.h>
// #include <openssl/err.h>
// #define PORT 443

void showLawrenceText(int x, int y)
{
    Rect r;
    unsigned int d = 0x00ffff44;
    r.bot = y-20;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, d, "lawrence marquez");
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
/* this is what i'm working on for lab 8. trying to get a pause screen working by adding font to update the 
class gameFont 
{
public:

//constructor
DBZGameFont();
//destructor
~DBZGameFont();

    void CreateFont(const string& strTypeface , int iFontHeight, int iFontWeight) {

    }
    void DrawText(const string& strText, int xPos, int yPos, GLfloat fRed, GLfloat fGreen, 
          GLfloat fBlue);
    
    TRectanglei GetTextSize(const std::string& strText);

    static void SetDeviceContext(HDC hDevContext)  
  { m_hDeviceContext = hDevContext; }

private:
  // The device context used to create the font.
  static HDC m_hDeviceContext;
  // The index of the base of the lists.
  GLuint m_uiListBase;
  // The win32 font
  HFONT m_hFont;
};

void gameFont::CreateFont(const string& strTypeface, int iFontHeight, int iFontWeight ) {
    
    if (!m_hDeviceContext)
  {
    string strError = "Impossible to create the font: ";
    strError += strTypeface;
    throw CException(strError);
    return;
  }

  // Ask openGL to generate a contiguous set of 255 display lists.
  m_uiListBase = glGenLists(255);
    if (m_uiListBase == 0) {
        string strError = "Impossible to create the font: ";
        strError += strTypeface;
        throw CException(strError);
        return;
    }

    m_hFont = ::CreateFont(~iFontHeight, 0, 0, 0,  iFontWeight, FALSE, FALSE, FALSE, ANSI_CHARSET,OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH, strTypeface.c_str());)

    if(m_hFont == 0) {
        m_uiListBase = 0;
        string strError = "Impossible to create the font: ";
        strError += strTypeface;
        throw CException(strError);
        return;
    }
     // Select the newly create font into the device context (and save the previous
  // one).
  HFONT hOldFont = (HFONT)SelectObject(m_hDeviceContext, m_hFont);
  // Generate the font display list (for the 255 characters) starting
  // at display list m_uiListBase.
  wglUseFontBitmaps(m_hDeviceContext, 0, 255, m_uiListBase);  
  // Set the original font back in the device context
  SelectObject(m_hDeviceContext, hOldFont); 
}




void renderMainMenu() {
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.DBZtexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255, 255, 255, 255);


}
*/
