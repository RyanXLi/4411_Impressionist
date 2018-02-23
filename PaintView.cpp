//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"

#include <math.h>
#include <random>
#include <algorithm>


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6

extern float frand();

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);
    m_pBitmapBitstart = m_pDoc->m_ucBitmap +
        3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;


    if (autoDrawAsked) {
        autoDraw(m_pDoc->m_pUI->getSpacing(), m_pDoc->m_pUI->getSizeRand(), TRUE);
        autoDrawAsked = 0;
    }

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();

	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );

        //printf("source: (%d, %d)\n", source.x, source.y);
        //printf("target: (%d, %d)\n\n", target.x, target.y);

        
		printf("%i\n",eventToDo);
		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
            if (target.y < (m_nWindowHeight - m_pDoc->m_nPaintHeight) || target.x > m_pDoc->m_nPaintWidth) { break; }
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			break;
		case LEFT_MOUSE_DRAG:
            if (target.y < (m_nWindowHeight - m_pDoc->m_nPaintHeight) || target.x > m_pDoc->m_nPaintWidth) { break; }
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			break;
		case LEFT_MOUSE_UP:
            
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );

			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
            m_pDoc->handleRightMouseDown(target);
			break;
		case RIGHT_MOUSE_DRAG:
            m_pDoc->handleRightMouseDrag(target);
			break;
		case RIGHT_MOUSE_UP:
            m_pDoc->handleRightMouseUp(target);
			break;

		default:
			printf("Unknown event!!\n");		
			break;
		}


	}

  

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{
    // ADDED
    int sourceX = 0;
    int sourceY = 0;
    // ADDED END

	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();

        // ADDED
        sourceX = coord.x + m_nStartCol;
        sourceY = m_nEndRow - coord.y;
        if (sourceX >= 0 && sourceX < m_pDoc->m_screenWidth 
            && sourceY >= 0 && sourceY < m_pDoc->m_screenHeight) {
            m_pDoc->m_pUI->m_origView->needToDrawDot = TRUE;
            m_pDoc->m_pUI->m_origView->brushLocation.x = Fl::event_x();
            m_pDoc->m_pUI->m_origView->brushLocation.y = Fl::event_y();
            m_pDoc->m_pUI->m_origView->redraw();
        }
        // ADDED END

		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();

        // ADDED
        sourceX = coord.x + m_nStartCol;
        sourceY = m_nEndRow - coord.y;
        if (sourceX >= 0 && sourceX < m_pDoc->m_screenWidth
            && sourceY >= 0 && sourceY < m_pDoc->m_screenHeight) {
            m_pDoc->m_pUI->m_origView->needToDrawDot = TRUE;
            m_pDoc->m_pUI->m_origView->brushLocation.x = Fl::event_x();
            m_pDoc->m_pUI->m_origView->brushLocation.y = Fl::event_y();
            m_pDoc->m_pUI->m_origView->redraw();
        }
        // ADDED END

		break;
	default:
		return 0;
		break;

	}



	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );

    if (needToExchange) {
    	if(m_pBitmapBitstart)
    		m_pPaintBackupBitstart=new unsigned char[3*m_nDrawWidth*m_nDrawHeight];
    		memcpy(m_pPaintBackupBitstart,m_pBitmapBitstart,3*m_nDrawWidth*m_nDrawHeight);
        glReadPixels(0,
            m_nWindowHeight - m_nDrawHeight,
            m_nDrawWidth,
            m_nDrawHeight,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            m_pBitmapBitstart);
    }
    else {
    	if (m_pPaintBitstart)
    		m_pPaintBackupBitstart=new unsigned char[3*m_nDrawWidth*m_nDrawHeight];
    		memcpy(m_pPaintBackupBitstart,m_pPaintBitstart,3*m_nDrawWidth*m_nDrawHeight);
        glReadPixels(0,
            m_nWindowHeight - m_nDrawHeight,
            m_nDrawWidth,
            m_nDrawHeight,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            m_pPaintBitstart);
    }
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);
    
	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	if (needToUndo){
		if(!m_pPaintBackupBitstart){
			needToUndo=false;
	        glDrawPixels(m_nDrawWidth,
	            m_nDrawHeight,
	            GL_RGB,
	            GL_UNSIGNED_BYTE,
	            m_pPaintBitstart);
			return;
		}
        glDrawPixels(m_nDrawWidth,
            m_nDrawHeight,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            m_pPaintBackupBitstart);
        if(needToExchange){
        	memcpy(m_pBitmapBitstart,
        			m_pPaintBackupBitstart,
        			3*m_nDrawWidth*m_nDrawHeight);
        }
        else{
        	memcpy(m_pPaintBitstart,
        			m_pPaintBackupBitstart,
        			3*m_nDrawWidth*m_nDrawHeight);
        }
        needToUndo=false;
        delete m_pPaintBackupBitstart;
        m_pPaintBackupBitstart=NULL;
        return;
	}
    if (needToExchange) {
        glDrawPixels(m_nDrawWidth,
            m_nDrawHeight,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            m_pBitmapBitstart);
    }
    else {
        glDrawPixels(m_nDrawWidth,
            m_nDrawHeight,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            m_pPaintBitstart);
    }

//	glDrawBuffer(GL_FRONT);
}

void PaintView::autoDraw(int spacing, bool sizeRand, bool orderRand) {
    // auto drawing
    //printf("drawH: %d\n", m_nDrawHeight);
    //printf("windH: %d\n", m_nWindowHeight);
    //printf("paintH: %d\n", m_pDoc->m_nPaintHeight);
    //printf("\n");

    int originalSize = m_pDoc->getSize();
    int sizeVar = 5;
    int numRequiredPoints = (m_pDoc->m_screenWidth / spacing + 1) * (m_pDoc->m_screenHeight / spacing + 1);  
    int* coords;

    if (orderRand) {

        coords = new int[numRequiredPoints];
        int t = 0; //indices

        int iMax = m_pDoc->m_screenWidth;
        int jMax = m_pDoc->m_screenHeight + (m_nWindowHeight - m_pDoc->m_nPaintHeight);

        for (int i = 0; i <= iMax; i += spacing) {
            for (int j = m_nWindowHeight - m_pDoc->m_nPaintHeight; j <= jMax; j += spacing) {
                coords[t] = i * (jMax + 1) + j;
                t++;
            }
        }
        knuthShuffle(coords, numRequiredPoints);


        for (t = 0; t < numRequiredPoints; t++) {
            int i = coords[t] / (jMax + 1);
            int j = coords[t] % (jMax + 1);

            //printf("i: %d\n", i);
            //printf("j: %d\n", j);
            if (sizeRand) {
                int newSize = originalSize + (frand() - 0.5) * sizeVar;
                while (newSize < 1) {
                    newSize = originalSize + (frand() - 0.5) * sizeVar;
                }
                m_pDoc->m_pUI->setSize(newSize);
            }
            int realY = j - (m_nWindowHeight - m_pDoc->m_nPaintHeight);
            m_pDoc->m_pCurrentBrush->BrushBegin({ i, realY }, { i, j });

            if (t % (m_pDoc->m_screenWidth / spacing + 1) == 0) {
                glFlush();
            }
        }
    }
    else {
        // normal order
        for (int i = 0; i <= m_pDoc->m_screenWidth; i += spacing) {
            for (int j = m_nWindowHeight - m_pDoc->m_nPaintHeight; j <= m_pDoc->m_screenHeight + (m_nWindowHeight - m_pDoc->m_nPaintHeight); j += spacing) {
                //printf("i: %d\n", i);
                //printf("j: %d\n", j);
                if (sizeRand) {
                    int newSize = originalSize + (frand() - 0.5) * sizeVar;
                    while (newSize < 1) {
                        newSize = originalSize + (frand() - 0.5) * sizeVar;
                    }
                    m_pDoc->m_pUI->setSize(newSize);
                }
                int realY = j - (m_nWindowHeight - m_pDoc->m_nPaintHeight);
                m_pDoc->m_pCurrentBrush->BrushBegin({ i, realY }, { i, j });
        
            }
            glFlush();
        }
    }

    m_pDoc->m_pUI->setSize(originalSize);

    SaveCurrentContent();
    RestoreContent();
}



void PaintView::knuthShuffle(int* array, int len) {

    std::random_device device;
    std::mt19937 mt(device());

    for (int i = len - 1; i >= 0; i--) {
        std::uniform_int_distribution<> dis(0, i);
        int rand = dis(mt);
        if (array[rand] != array[i]) {
            std::swap(array[rand], array[i]);
        }
    }

}


GLubyte* PaintView::cacheForExchange() {

    glReadBuffer(GL_FRONT);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);

    GLubyte* exchangeCache = (GLubyte*)malloc(3 * m_pDoc->m_screenWidth * m_pDoc->m_screenHeight);
    if (m_pDoc->hasDrawn) {
        glReadPixels(0,
            m_nWindowHeight - m_nDrawHeight,
            m_nDrawWidth,
            m_nDrawHeight,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            exchangeCache);
    }
    else {
        memset(exchangeCache, 0, 3 * m_pDoc->m_screenWidth * m_pDoc->m_screenHeight);
    }

    return exchangeCache;
}
