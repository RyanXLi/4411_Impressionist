//
// originalview.h
//
// The header file for original view of the input images
//

#ifndef ORIGINALVIEW_H
#define ORIGINALVIEW_H

#include <FL/Fl.H>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>

#include "ImpBrush.h"
#include "impressionistDoc.h"

class ImpressionistDoc;

class OriginalView : public Fl_Gl_Window
{
public:
	OriginalView(int x, int y, int w, int h, const char *l);
	
	void draw();
	void refresh();

	void resizeWindow(int width, int height);
    
    GLubyte* cacheForExchange();
    void SaveCurrentContent();
    void RestoreContent();


	ImpressionistDoc*	m_pDoc;

    Point brushLocation;
    bool needToDrawDot = FALSE;
    bool needToExchange = FALSE;

    GLubyte* originalViewExchangeCache = nullptr;
    GLubyte* contentCache = nullptr;


private:
    int	m_nWindowWidth,
        m_nWindowHeight,
        m_nDrawWidth,
        m_nDrawHeight;
public:
    int displayImage = DISPLAY_MAIN;

};

#endif