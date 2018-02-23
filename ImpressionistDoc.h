// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"
#include <vector>


enum {
    DISPLAY_MAIN = 0,
    DISPLAY_OTHER,
    DISPLAY_EDGE
};

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
    void    setStrokeDirection(int type);

	int		getSize();						// get the UI size
    //void	setSize(int size);				// set the UI size

    int     getLineWidth();
    //void    setLineWidth(int lineWidth);

    int     getLineAngle();
    //void    setLineAngle(int lineAngle);

    double  getAlpha();
    //void    setAlpha(double alpha);

	char*	getImageName();					// get the current image name
	
    void handleRightMouseDown(Point target);  // for changing line angle by dragging
    void handleRightMouseDrag(Point target);
    void handleRightMouseUp(Point target);

    //void autoDraw(int spacing, bool sizeRand, bool orderRand);

    int applyMatrix(Point source, std::vector<std::vector<int>> matrix, int matrixDim, bool useWeightSum, bool useGetOtherPixel);
    int applyMatrixToMatrix(std::vector<std::vector<int>> originalMatrix, std::vector<std::vector<int>> matrix, int matrixDim, bool useWeightSum);

    void disolve();

    GLuint intensity(Point point, bool useGetOtherPixel) {

        if (useGetOtherPixel) {
            GLubyte red = (GetOtherPixel(point.x, point.y))[0];
            GLubyte green = (GetOtherPixel(point.x, point.y))[1];
            GLubyte blue = (GetOtherPixel(point.x, point.y))[2];
            return 0.299 * red + 0.587 * green + 0.114 * blue;
        }
        else {
            GLubyte red = (GetOriginalPixel(point))[0];
            GLubyte green = (GetOriginalPixel(point))[1];
            GLubyte blue = (GetOriginalPixel(point))[2];
            return 0.299 * red + 0.587 * green + 0.114 * blue;
        }
    }

    int loadAnotherImage(char *iname);
    int loadEdgeImage(char *iname);



// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;
    unsigned char*  m_ucOtherBitmap;
    unsigned char*  m_ucEdgeBitmap;

    bool mainImageLoaded = false;
    bool otherImageLoaded = false;
    bool edgeImageLoaded = false;


	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
    int                 m_pCurrentStrokeDirection; // equals type enum

	// Size of the brush.
	int m_nSize;							

	ImpressionistUI*	m_pUI;

    bool hasDrawn;

    std::vector<std::vector<int>> matrices[2] = {
        { {1, 2, 1},
          {2, 4, 2},
          {1, 2, 1}
        }, // gaussian blur

        { {0, -1, 0},
          {-1, 5, -1},
          {0, -1, 0}
        } // sharpening
    };

    std::vector<std::vector<int>> curMatrix = matrices[FILTER_GAUSSIAN_BLUR];


// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
    GLubyte* GetOtherPixel(int x, int y);
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  


private:
	char			m_imageName[256];
    Point* rightMouseStartPoint = nullptr;
    Point* rightMouseCurPoint = nullptr;
    Point* rightMouseEndPoint = nullptr;
    GLubyte* framebufferCache = nullptr;

public:
    int m_screenWidth = 600;
    int m_screenHeight = 300; // default values, TODO: getter setter

};


extern void MessageBox(char *message);

#endif
