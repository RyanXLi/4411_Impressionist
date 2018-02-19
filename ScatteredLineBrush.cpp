//
// ScatteredLineBrush.cpp
//


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredLineBrush.h"

extern float frand();

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) :
    ImpBrush(pDoc, name) {
}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;
    pDoc->hasDrawn = 1;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int lineWidth = pDoc->getLineWidth();
    
    glLineWidth(lineWidth);

    BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    if (pDoc == NULL) {
        printf("ScatteredLineBrush::BrushMove  document is NULL\n");
        return;
    }

    glPushMatrix();
    SetColorWithAlpha(source);

    int size = pDoc->getSize();

    glTranslatef(target.x, target.y, 0);
    glRotatef(pDoc->getLineAngle(), 0.0, 0.0, 1.0);



    glBegin(GL_LINES);

    int numLinesPerBlock = 2 + frand() * 4;

    for (int i = 0; i < numLinesPerBlock; i++) {
        double height = (frand() - 0.5) * size;

        double oneEnd = (frand() - 0.5) * size;
        double otherEnd = (frand() - 0.5) * size;

        // TODO: out of bound

        Point* pSource = new Point(source.x + 0.5 * (oneEnd + otherEnd),
            source.y + height);
        SetColorWithAlpha(*pSource);

        glVertex2d(oneEnd, height);
        glVertex2d(otherEnd, height);

    }

    glEnd();
    glPopMatrix();
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}



