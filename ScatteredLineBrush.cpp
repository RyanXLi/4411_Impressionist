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
        double height = (frand() - 0.5) * size * 2;

        double leftEnd = 0.5 * frand() * size * 2;
        double rightEnd = -0.5 * frand() * size * 2;

        // TODO: out of bound

        SetColorWithAlpha({source.x, source.y + (int)height});

        glVertex2d(leftEnd, height);
        glVertex2d(rightEnd, height);

    }

    glEnd();
    glPopMatrix();
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}



