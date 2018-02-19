//
// CircleBrush.cpp
//


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CircleBrush.h"
#include <math.h>


extern float frand();
extern int NUM_OF_TRIANGLE;

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
    ImpBrush(pDoc, name) {
}

void CircleBrush::BrushBegin(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    if (pDoc == NULL) {
        printf("CircleBrush::BrushMove  document is NULL\n");
        return;
    }


    SetColorWithAlpha(source);

    int size = pDoc->getSize();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(target.x, target.y); // center
    for (int i = 0; i <= NUM_OF_TRIANGLE; i++) {
        glVertex2f(
            target.x + (0.5 * size * cos(2.0 * M_PI * i / NUM_OF_TRIANGLE)),
            target.y + (0.5 * size * sin(2.0 * M_PI * i / NUM_OF_TRIANGLE))
        );
    }
    glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}



