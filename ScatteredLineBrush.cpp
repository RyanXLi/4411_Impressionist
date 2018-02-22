//
// ScatteredLineBrush.cpp
//


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredLineBrush.h"
#include <math.h>


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

    firstClick = true;
    lastCoor.x = source.x;
    lastCoor.y = source.y;

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

    if (pDoc->m_pCurrentStrokeDirection == DIRECTION_BRUSH_DIRECTION) {
        if (firstClick) {
            firstClick = false;  
        }
        else {
            int angle = (int)(atan2(source.y - lastCoor.y, source.x - lastCoor.x) * 180 / M_PI);
            pDoc->m_pUI->setLineAngle(angle);
            lastCoor.x = source.x;
            lastCoor.y = source.y;
        }
    }
    else if (pDoc->m_pCurrentStrokeDirection == DIRECTION_GRADIENT) {

    }

    glRotatef(pDoc->getLineAngle(), 0.0, 0.0, 1.0);

    glBegin(GL_LINES);

    int numLinesPerBlock = 2 + frand() * 4;

    for (int i = 0; i < numLinesPerBlock; i++) {
        double x = (frand() - 0.5) * size;
        double y = (frand() - 0.5) * size;

        SetColorWithAlpha({source.x + (int)x, source.y + (int)y});

        glVertex2d(x - size * 0.5, y);
        glVertex2d(x + size * 0.5, y);

    }

    glEnd();
    glPopMatrix();
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}



