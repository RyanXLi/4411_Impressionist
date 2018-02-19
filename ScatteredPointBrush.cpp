//
// ScatteredPointBrush.cpp
//


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointBrush.h"

extern float frand();

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name) :
    ImpBrush(pDoc, name) {
}

void ScatteredPointBrush::BrushBegin(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    pDoc->hasDrawn = 1;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BrushMove(source, target);
}

void ScatteredPointBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    if (pDoc == NULL) {
        printf("ScatteredPointBrush::BrushMove  document is NULL\n");
        return;
    }

    int size = pDoc->getSize();

    glBegin(GL_POINTS);

    for (int i = 0; i < size * size * 0.2; i++) {

        double xVar = (frand() - 0.5) * size;
        double yVar = (frand() - 0.5) * size;

        Point* pSource = new Point(source.x + xVar, source.y + yVar);
        Point* pTarget = new Point(target.x + xVar, target.y + yVar);

        SetColorWithAlpha(*pSource);
        glVertex2d(pTarget->x, pTarget->y);
    }

    glEnd();
}

void ScatteredPointBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}



