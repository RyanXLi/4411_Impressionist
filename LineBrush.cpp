//
// LineBrush.cpp
//


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"
#include <math.h>
#include <vector>

extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
    ImpBrush(pDoc, name) {
}

void LineBrush::BrushBegin(const Point source, const Point target) {
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

void LineBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    if (pDoc == NULL) {
        printf("LineBrush::BrushMove  document is NULL\n");
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
        // apply matrix: gaussianBlur
        std::vector<std::vector<int>> gaussianBlur = { 
            {1, 2, 1}, 
            {2, 4, 2}, 
            {1, 2, 1} 
        };
        std::vector<std::vector<int>> blurred = { 
            { pDoc->applyMatrix({ source.x - 1, source.y + 1 }, gaussianBlur, 3, TRUE),
              pDoc->applyMatrix({ source.x,     source.y + 1 }, gaussianBlur, 3, TRUE),
              pDoc->applyMatrix({ source.x + 1, source.y + 1 }, gaussianBlur, 3, TRUE) },

            { pDoc->applyMatrix({ source.x - 1, source.y}, gaussianBlur, 3, TRUE),
              pDoc->applyMatrix(source                   , gaussianBlur, 3, TRUE),
              pDoc->applyMatrix({ source.x + 1, source.y}, gaussianBlur, 3, TRUE) },

            { pDoc->applyMatrix({ source.x - 1, source.y - 1 }, gaussianBlur, 3, TRUE),
              pDoc->applyMatrix({ source.x    , source.y - 1 }, gaussianBlur, 3, TRUE),
              pDoc->applyMatrix({ source.x + 1, source.y - 1 }, gaussianBlur, 3, TRUE) }
        };
        // apply matrix: sobelX, sobelY
        std::vector<std::vector<int>> sobelX = {
            { 1, 0, -1 },
            { 2, 0, -2 },
            { 1, 0, -1 }
        };
        std::vector<std::vector<int>> sobelY = {
            { 1, 2, 1 },
            { 0, 0, 0 },
            { -1, -2, -1 }
        };


        int sobelXresult = pDoc->applyMatrixToMatrix(blurred, sobelX, 3, FALSE);
        int sobelYresult = pDoc->applyMatrixToMatrix(blurred, sobelY, 3, FALSE);
        

        pDoc->m_pUI->setLineAngle(atan2(sobelYresult, sobelXresult) * 180 / M_PI + 90);
    }


    glRotatef(pDoc->getLineAngle(), 0.0, 0.0, 1.0);

    glBegin(GL_LINES);

    glVertex2d(- size / 2, 0);
    glVertex2d(size / 2, 0);

    glEnd();
    glPopMatrix();
}

void LineBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}



