//
// paintview.h
//
// The header file for painting view of the input images
//

#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

class ImpressionistDoc;

class PaintView : public Fl_Gl_Window
{
public:
	PaintView(int x, int y, int w, int h, const char* l);
	void draw();
	int handle(int event);

	void refresh();
	
	void resizeWindow(int width, int height);

	void SaveCurrentContent();

	void RestoreContent();

    void autoDraw(int spacing, bool sizeRand, bool orderRand);

    GLubyte* cacheForExchange();

private:
    void knuthShuffle(int* array, int len);

public:

	ImpressionistDoc *m_pDoc;

    bool autoDrawAsked = 0;

    int xToDraw = 0;
    int yToDraw = 0;

    GLubyte* paintViewExchangeCache = nullptr;
    bool needToExchange = FALSE;
    bool needToUndo = FALSE;


//private: // TODO: change back
	GLvoid* m_pPaintBitstart;
    GLvoid* m_pBitmapBitstart;
    GLvoid* m_pPaintBackupBitstart=NULL;
	int		m_nDrawWidth,
			m_nDrawHeight,
			m_nStartRow, 
			m_nEndRow,
			m_nStartCol, 
			m_nEndCol,
			m_nWindowWidth, 
			m_nWindowHeight;


};

#endif
