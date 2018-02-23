//
// FilterBrush.cpp
//


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "FilterBrush.h"
#include <math.h>



extern float frand();
extern int NUM_OF_TRIANGLE;

FilterBrush::FilterBrush(ImpressionistDoc* pDoc, char* name) :
    ImpBrush(pDoc, name) {

    setMatrix({ {1,1,1}, {1,1,1}, {1,1,1} }, 3);
}

void FilterBrush::BrushBegin(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    if (matrixDim == 0) { return; }

    pDoc->hasDrawn = 1;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BrushMove(source, target);
}

void FilterBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    if (matrixDim == 0) { return; }

    if (pDoc == NULL) {
        printf("FilterBrush::BrushMove  document is NULL\n");
        return;
    }

    // suppose input valid
    int weightSum = 0;

    for (int i = 0; i < matrixDim; i++) {
        for (int j = 0; j < matrixDim; j++) {
            weightSum += matrix[i][j];
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
            else if (x < 0 && y >= pDoc->m_screenHeight) {
                pxColor = intensity(Point(0, pDoc->m_screenHeight - 1));
            }
            else if (x >= pDoc->m_screenWidth && y < 0) {
                pxColor = intensity(Point(pDoc->m_screenWidth - 1, 0));
            }
            else if (x >= pDoc->m_screenWidth && y >= pDoc->m_screenHeight) {
                pxColor = intensity(Point(pDoc->m_screenWidth - 1, pDoc->m_screenHeight - 1));
            }
            else if (x < 0) {
                pxColor = intensity(Point(0, y));
            }
            else if (x >= pDoc->m_screenWidth) {
                pxColor = intensity(Point(pDoc->m_screenWidth - 1, y));
            }
            else if (y < 0) {
                pxColor = intensity(Point(x, 0));
            }
            else if (y >= pDoc->m_screenHeight) {
                pxColor = intensity(Point(x, pDoc->m_screenHeight - 1));
            }
            else {
                pxColor = intensity(Point(x, y));
            }

            finalColor += pxColor * matrix[i][j];
        }
    }

    finalColor /= weightSum;

    glColor3ub((GLubyte)finalColor, (GLubyte)finalColor, (GLubyte)finalColor);

    glBegin(GL_POINTS);
    glVertex2f(target.x, target.y); 
    glEnd();
}

void FilterBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}



