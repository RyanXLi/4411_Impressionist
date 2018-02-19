//
// ScatteredCircleBrush.cpp
//


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCircleBrush.h"
#include <math.h>


extern float frand();
extern int NUM_OF_TRIANGLE;

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
    ImpBrush(pDoc, name) {
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int lineWidth = pDoc->getLineWidth();
    
    glLineWidth(lineWidth);

    BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    if (pDoc == NULL) {
        printf("ScatteredCircleBrush::BrushMove  document is NULL\n");
        return;
    }

    int size = pDoc->getSize();
    int numCirclesPerBlock = 3;

    for (int i = 0; i < numCirclesPerBlock; i++) {

        double xVar = (frand() - 0.5) * size;
        double yVar = (frand() - 0.5) * size;

        Point* pSource = new Point(source.x + xVar, source.y + yVar);
        SetColorWithAlpha(*pSource);

        Point* center = new Point(target.x + xVar, target.y + yVar);// TODO

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(center->x, center->y); // center
        for (int i = 0; i <= NUM_OF_TRIANGLE; i++) {
            glVertex2f(
                center->x + (0.5 * size * cos(2.0 * M_PI * i / NUM_OF_TRIANGLE)),
                center->y + (0.5 * size * sin(2.0 * M_PI * i / NUM_OF_TRIANGLE))
            );
        }
        glEnd();
    }
}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}



