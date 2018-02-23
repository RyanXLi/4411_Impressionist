//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "originalview.h"

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#endif

OriginalView::OriginalView(int			x, 
						   int			y, 
						   int			w, 
						   int			h, 
						   const char*	l)
							: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}

void OriginalView::draw()
{
	if(!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer( GL_FRONT );
		ortho();

	}

	glClear( GL_COLOR_BUFFER_BIT );

	if ( m_pDoc->m_ucBitmap ) 
	{
		// note that both OpenGL pixel storage and the Windows BMP format
		// store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
		// around with startrow.

		m_nWindowWidth=w();
		m_nWindowHeight=h();

		int drawWidth, drawHeight;
		GLvoid* bitstart;
        GLvoid* paintstart;
        GLvoid* otherBitStart;
        GLvoid* edgeBitStart;

		// we are not using a scrollable window, so ignore it
		Point scrollpos;	// = GetScrollPosition();
		scrollpos.x=scrollpos.y=0;

		drawWidth	= min( m_nWindowWidth, m_pDoc->m_nWidth );
		drawHeight	= min( m_nWindowHeight, m_pDoc->m_nHeight );

        m_nDrawWidth = drawWidth;
        m_nDrawHeight = drawHeight;

		int	startrow	= m_pDoc->m_nHeight - (scrollpos.y + drawHeight);
		if ( startrow < 0 ) 
			startrow = 0;


		bitstart = m_pDoc->m_ucBitmap + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);
        otherBitStart = m_pDoc->m_ucOtherBitmap + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);
        edgeBitStart = m_pDoc->m_ucEdgeBitmap + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);
        paintstart = m_pDoc->m_ucPainting + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);

		// just copy image to GLwindow conceptually
		glRasterPos2i( 0, m_nWindowHeight - drawHeight );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth );
		glDrawBuffer( GL_BACK );
        if (needToExchange) {
            glDrawPixels(drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, paintstart);
        }
        else {

            if (displayImage == DISPLAY_MAIN) {
                glDrawPixels(drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart);
            }
            else if (displayImage == DISPLAY_OTHER) {
                glDrawPixels(drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, otherBitStart);
            }
            else if (displayImage == DISPLAY_EDGE) {
                glDrawPixels(drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, edgeBitStart);
            }
        }
        if (needToDrawDot) {

            glPointSize(5.0);
            GLubyte color[4] = { 255,0,0,255 };
            glColor4ubv(color);

            glBegin(GL_POINTS);
            glVertex2d(brushLocation.x + scrollpos.x,
                m_nWindowHeight - brushLocation.y);
            glEnd();

        }


	}
			
	glFlush();
}

void OriginalView::refresh()
{
	redraw();
}

void OriginalView::resizeWindow(int	width, 
								int	height)
{
	resize(x(), y(), width, height);
}


GLubyte* OriginalView::cacheForExchange() {

    //originalViewExchangeCache = (GLubyte*)malloc(3 * m_pDoc->m_screenWidth * m_pDoc->m_screenHeight);
    //if (m_pDoc->hasDrawn) {
    //    glReadPixels(0, 0, m_pDoc->m_screenWidth, m_pDoc->m_screenHeight, GL_RGB, GL_UNSIGNED_BYTE, originalViewExchangeCache);
    //}
    //else {
    //    memset(originalViewExchangeCache, 0, 3 * m_pDoc->m_screenWidth * m_pDoc->m_screenHeight);
    //}

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);

    GLubyte* exchangeCache = (GLubyte*)malloc(3 * m_pDoc->m_screenWidth * m_pDoc->m_screenHeight);
    glReadPixels(0,
        0,
        m_nDrawWidth,
        m_nDrawHeight,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        exchangeCache);

    return exchangeCache;
}

void OriginalView::SaveCurrentContent() {
    // Tell openGL to read from the front buffer when capturing
    // out paint strokes

    contentCache = (GLubyte*)malloc(3 * m_pDoc->m_screenWidth * m_pDoc->m_screenHeight);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);

    glReadPixels(0,
        m_nWindowHeight - m_nDrawHeight,
        m_nDrawWidth,
        m_nDrawHeight,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        contentCache);
}


void OriginalView::RestoreContent() {

    glClear(GL_COLOR_BUFFER_BIT);

    glRasterPos2i(0, m_nWindowHeight - m_nDrawHeight);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
    glDrawPixels(m_nDrawWidth,
        m_nDrawHeight,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        contentCache);

    delete[] contentCache;

    //	glDrawBuffer(GL_FRONT);
}
