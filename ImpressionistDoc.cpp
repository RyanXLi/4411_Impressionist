// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"
#include <math.h>
#include <random>
#include <algorithm>

// Include individual brush headers here.
#include "PointBrush.h"
#include "LineBrush.h"
#include "CircleBrush.h"
#include "ScatteredPointBrush.h"
#include "ScatteredLineBrush.h"
#include "ScatteredCircleBrush.h"
#include "FilterBrush.h"


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }
extern int NUM_OF_TRIANGLE = 20;


ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatteredPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatteredLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatteredCircleBrush( this, "Scattered Circles" );
    ImpBrush::c_pBrushes[BRUSH_FILTER]
        = new FilterBrush(this, "Filter");

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];
    m_pCurrentStrokeDirection = DIRECTION_SLIDER_OR_RMOUSE;

    hasDrawn = 0;
}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}


void ImpressionistDoc::handleRightMouseDown(Point target) {

    if (m_pCurrentStrokeDirection != DIRECTION_SLIDER_OR_RMOUSE) { return; }
    //if (target.x < 0 || target.x >= m_screenWidth
    //    || target.y < m_nPaintHeight - m_pUI->m_paintView->m_nWindowHeight || target.y > m_nPaintHeight - m_pUI->m_paintView->m_nWindowHeight + m_screenHeight) {
    //    return;
    //}

    // cache framebuffer
    glReadBuffer(GL_FRONT);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ROW_LENGTH, m_nPaintWidth);

    framebufferCache = (GLubyte*)malloc(3 * m_screenWidth * m_screenHeight);


    if (hasDrawn) {
        glReadPixels(0,
            m_pUI->m_paintView->m_nWindowHeight - m_pUI->m_paintView->m_nDrawHeight,
            m_pUI->m_paintView->m_nDrawWidth,
            m_pUI->m_paintView->m_nDrawHeight,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            framebufferCache);
    }
    else {
        memset(framebufferCache, 0, 3 * m_screenWidth * m_screenHeight);
    }
    

    rightMouseStartPoint = new Point(target.x, target.y);

    glPointSize(3.0f);
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_POINTS);
    glVertex2f(target.x, target.y);
    glEnd();


}

void ImpressionistDoc::handleRightMouseDrag(Point target) {

    if (m_pCurrentStrokeDirection != DIRECTION_SLIDER_OR_RMOUSE) { return; }

    // clear current framebuffer
    // restore saved framebuffer
    glDrawBuffer(GL_BACK);

    glClear(GL_COLOR_BUFFER_BIT);

    glRasterPos2i(0, m_pUI->m_paintView->m_nWindowHeight - m_pUI->m_paintView->m_nDrawHeight);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, m_nPaintWidth);
    glDrawPixels(m_pUI->m_paintView->m_nDrawWidth,
        m_pUI->m_paintView->m_nDrawHeight,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        framebufferCache);

    if (rightMouseCurPoint != nullptr) {
        delete rightMouseCurPoint;
    }


    rightMouseCurPoint = new Point(target.x, target.y);

    glBegin(GL_POINTS);
    glVertex2f(target.x, target.y);
    glEnd();

    glLineWidth(1.0f);

    glBegin(GL_LINES);
    glVertex2f(rightMouseStartPoint->x, rightMouseStartPoint->y);
    glVertex2f(rightMouseCurPoint->x, rightMouseCurPoint->y);
    glEnd();

}

void ImpressionistDoc::handleRightMouseUp(Point target) {

    if (m_pCurrentStrokeDirection != DIRECTION_SLIDER_OR_RMOUSE) { return; }

    // clear current framebuffer
    // restore saved framebuffer
    glDrawBuffer(GL_BACK);

    glClear(GL_COLOR_BUFFER_BIT);

    glRasterPos2i(0, m_pUI->m_paintView->m_nWindowHeight - m_pUI->m_paintView->m_nDrawHeight);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, m_nPaintWidth);
    glDrawPixels(m_pUI->m_paintView->m_nDrawWidth,
        m_pUI->m_paintView->m_nDrawHeight,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        framebufferCache);

    rightMouseEndPoint = new Point(target.x, target.y);

    // calculate line angle
    Point* upperPoint = rightMouseEndPoint->y > rightMouseStartPoint->y ? 
        rightMouseEndPoint : rightMouseStartPoint;

    Point* lowerPoint = rightMouseEndPoint->y > rightMouseStartPoint->y ?
        rightMouseStartPoint : rightMouseEndPoint;

    m_pUI->setLineAngle(
        (int)(atan2(upperPoint->y - lowerPoint->y, upperPoint->x - lowerPoint->x) * 180 / M_PI));
}



int ImpressionistDoc::applyMatrix(Point source, std::vector<std::vector<int>> matrix, int matrixDim, bool useWeightSum) {
    // suppose input valid
    int weightSum = 0;

    if (useWeightSum) {
        for (int i = 0; i < matrixDim; i++) {
            for (int j = 0; j < matrixDim; j++) {
                weightSum += matrix[i][j];
            }
        }
    }

    GLuint pxColor = 0;
    GLuint finalColor = 0;

    for (int i = 0; i < matrixDim; i++) {
        for (int j = 0; j < matrixDim; j++) {
            int y = source.y + i - (matrixDim - 1) / 2;
            int x = source.x + j - (matrixDim - 1) / 2;

            if (x < 0 && y < 0) {
                pxColor = intensity(Point(0, 0));
            }
            else if (x < 0 && y >= m_screenHeight) {
                pxColor = intensity(Point(0, m_screenHeight - 1));
            }
            else if (x >= m_screenWidth && y < 0) {
                pxColor = intensity(Point(m_screenWidth - 1, 0));
            }
            else if (x >= m_screenWidth && y >= m_screenHeight) {
                pxColor = intensity(Point(m_screenWidth - 1, m_screenHeight - 1));
            }
            else if (x < 0) {
                pxColor = intensity(Point(0, y));
            }
            else if (x >= m_screenWidth) {
                pxColor = intensity(Point(m_screenWidth - 1, y));
            }
            else if (y < 0) {
                pxColor = intensity(Point(x, 0));
            }
            else if (y >= m_screenHeight) {
                pxColor = intensity(Point(x, m_screenHeight - 1));
            }
            else {
                pxColor = intensity(Point(x, y));
            }

            finalColor += pxColor * matrix[i][j];
        }
    }

    if (useWeightSum) finalColor /= weightSum;

    return finalColor;
}


int ImpressionistDoc::applyMatrixToMatrix(std::vector<std::vector<int>> originalMatrix, std::vector<std::vector<int>> matrix, int matrixDim, bool useWeightSum) {
    // suppose input valid
    // assume `originalMatrix` and `matrix` both have dimension `matrixDim`


    int weightSum = 0;

    if (useWeightSum) {
        for (int i = 0; i < matrixDim; i++) {
            for (int j = 0; j < matrixDim; j++) {
                weightSum += matrix[i][j];
            }
        }
    }

    GLuint pxColor = 0;
    GLuint finalColor = 0;

    for (int i = 0; i < matrixDim; i++) {
        for (int j = 0; j < matrixDim; j++) {
            int y = i;
            int x = j;

            finalColor += originalMatrix[x][y] * matrix[i][j];
        }
    }
    
    if (useWeightSum) finalColor /= weightSum;

    return finalColor;
}




//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];


    switch (type) {

        case BRUSH_POINTS:
            setStrokeDirection(DIRECTION_SLIDER_OR_RMOUSE);
            m_pUI->m_StrokeDirectionChoice->deactivate();
            m_pUI->m_BrushSizeSlider->activate();
            m_pUI->setLineWidth(1);
            m_pUI->m_LineWidthSlider->deactivate();
            m_pUI->setLineAngle(0);
            m_pUI->m_LineAngleSlider->deactivate();
            m_pUI->m_AlphaSlider->activate();
            break;

        case BRUSH_LINES:
            m_pUI->m_StrokeDirectionChoice->activate();
            m_pUI->m_BrushSizeSlider->activate();
            m_pUI->m_LineWidthSlider->activate();
            m_pUI->m_LineAngleSlider->activate();
            m_pUI->m_AlphaSlider->activate();
            break;

        case BRUSH_CIRCLES:
            setStrokeDirection(DIRECTION_SLIDER_OR_RMOUSE);
            m_pUI->m_StrokeDirectionChoice->deactivate();
            m_pUI->m_BrushSizeSlider->activate();
            m_pUI->setLineWidth(1);
            m_pUI->m_LineWidthSlider->deactivate();
            m_pUI->setLineAngle(0);
            m_pUI->m_LineAngleSlider->deactivate();
            m_pUI->m_AlphaSlider->activate();
            break;

        case BRUSH_SCATTERED_POINTS:
            setStrokeDirection(DIRECTION_SLIDER_OR_RMOUSE);
            m_pUI->m_StrokeDirectionChoice->deactivate();
            m_pUI->m_BrushSizeSlider->activate();
            m_pUI->setLineWidth(1);
            m_pUI->m_LineWidthSlider->deactivate();
            m_pUI->setLineAngle(0);
            m_pUI->m_LineAngleSlider->deactivate();
            m_pUI->m_AlphaSlider->activate();
            break;

        case BRUSH_SCATTERED_LINES:
            m_pUI->m_StrokeDirectionChoice->activate();
            m_pUI->m_BrushSizeSlider->activate();
            m_pUI->m_LineWidthSlider->activate();
            m_pUI->m_LineAngleSlider->activate();
            m_pUI->m_AlphaSlider->activate();
            break;

        case BRUSH_SCATTERED_CIRCLES:
            setStrokeDirection(DIRECTION_SLIDER_OR_RMOUSE);
            m_pUI->m_StrokeDirectionChoice->deactivate();
            m_pUI->m_BrushSizeSlider->activate();
            m_pUI->setLineWidth(1);
            m_pUI->m_LineWidthSlider->deactivate();
            m_pUI->setLineAngle(0);
            m_pUI->m_LineAngleSlider->deactivate();
            m_pUI->m_AlphaSlider->activate();
            break;

        case BRUSH_FILTER:
            setStrokeDirection(DIRECTION_SLIDER_OR_RMOUSE);
            m_pUI->m_StrokeDirectionChoice->deactivate();
            m_pUI->setSize(1);
            m_pUI->m_BrushSizeSlider->deactivate();
            m_pUI->setLineWidth(1);
            m_pUI->m_LineWidthSlider->deactivate();
            m_pUI->setLineAngle(0);
            m_pUI->m_LineAngleSlider->deactivate();
            m_pUI->setAlpha(0.0);
            m_pUI->m_AlphaSlider->deactivate();
            break;

        default:
            break;
    }
}

void ImpressionistDoc::setStrokeDirection(int type) {
    m_pCurrentStrokeDirection = type;
}


//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

int ImpressionistDoc::getLineWidth() {
    return m_pUI->getLineWidth();
}

int ImpressionistDoc::getLineAngle() {
    return m_pUI->getLineAngle();
}

double ImpressionistDoc::getAlpha() {
    return m_pUI->getAlpha();
}




//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

    // ADDED
    m_screenHeight = height;
    m_screenWidth = width;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
    m_pUI->m_origView->displayImage = DISPLAY_MAIN;
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();

    mainImageLoaded = TRUE;

	return 1;
}


int ImpressionistDoc::loadAnotherImage(char *iname) {

    // try to open the image to read
    unsigned char*	data;
    int				width,
        height;

    if ((data = readBMP(iname, width, height)) == NULL) {
        fl_alert("Can't load bitmap file");
        return 0;
    }

    // reflect the fact of loading the new image
    if (m_nWidth != width || m_nHeight != height) {
        fl_alert("Different image dimension");
        return 0;
    }


    //// release old storage
    //if (m_ucBitmap) delete[] m_ucBitmap;
    //if (m_ucPainting) delete[] m_ucPainting;

    m_ucOtherBitmap = data;

    // allocate space for draw view
    //m_ucPainting = new unsigned char[width*height * 3];
    //memset(m_ucPainting, 0, width*height * 3);
    //
    //m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
    //    m_pUI->m_mainWindow->y(),
    //    width * 2,
    //    height + 25);

    // display it on origView
    m_pUI->m_origView->displayImage = DISPLAY_OTHER;
    m_pUI->m_origView->refresh();

    otherImageLoaded = true;

    return 1;
}


int ImpressionistDoc::loadEdgeImage(char *iname) {

    // try to open the image to read
    unsigned char*	data;
    int				width,
        height;

    if ((data = readBMP(iname, width, height)) == NULL) {
        fl_alert("Can't load bitmap file");
        return 0;
    }

    // reflect the fact of loading the new image
    if (m_nWidth != width || m_nHeight != height) {
        fl_alert("Different image dimension");
        return 0;
    }

    m_ucEdgeBitmap = data;


    // display it on origView
    m_pUI->m_origView->displayImage = DISPLAY_EDGE;
    m_pUI->m_origView->refresh();

    edgeImageLoaded = true;

    return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

    GLubyte* targetPixel;
    if (m_pUI->m_paintView->needToExchange) {
        targetPixel = (GLubyte*)(m_ucPainting + 3 * (y*m_nWidth + x));
    }
    else if (m_pUI->m_origView->displayImage == DISPLAY_OTHER) {
        targetPixel = (GLubyte*)(m_ucOtherBitmap + 3 * (y*m_nWidth + x));
    }
    else //DISPLAY_MAIN or EDGE
    {
        targetPixel = (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
    }


    GLubyte* processedPixel = new GLubyte[3]{ (GLubyte)min((targetPixel[0] * m_pUI->m_red), 255),
        (GLubyte)min((targetPixel[1] * m_pUI->m_green), 255),
        (GLubyte)min((targetPixel[2] * m_pUI->m_blue), 255) };
    return processedPixel;
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}


void ImpressionistDoc::disolve() {
    //disolve main and the other image
    if (!mainImageLoaded) {
        fl_alert("Main image not loaded");
        return;
    }
    if (!otherImageLoaded) {
        fl_alert("Other image not loaded");
        return;
    }

    for (int i = 0; i < 3 * m_nPaintWidth*m_nPaintHeight; i++) {
        *(m_ucPainting + i) = *(m_ucBitmap + i) * 0.5 + *(m_ucOtherBitmap + i) * 0.5;
    }

    m_pUI->m_paintView->redraw();
}

