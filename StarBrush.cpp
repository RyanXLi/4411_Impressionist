/*
 * StarBrush.cpp
 *
 *  Created on: 2018Äê2ÔÂ23ÈÕ
 *      Author: wyb
 */

//
// CircleBrush.cpp
//


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "StarBrush.h"
#include <math.h>


extern float frand();
extern int NUM_OF_TRIANGLE;

StarBrush::StarBrush(ImpressionistDoc* pDoc, char* name) :
    ImpBrush(pDoc, name) {
}


void StarBrush::BrushBegin(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    pDoc->hasDrawn = 1;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BrushMove(source, target);
}

void StarBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    if (pDoc == NULL) {
        printf("CircleBrush::BrushMove  document is NULL\n");
        return;
    }


    SetColorWithAlpha(source);

    int size = pDoc->getSize();

    if (size == 1) {
        glPointSize(1);
        glBegin(GL_POINTS);
        glVertex2f(target.x, target.y);
        glEnd();
        return;
    }

    glBegin(GL_POLYGON);
    glVertex2f(target.x, target.y); // center
    for (int i = 0; i <= 10; i++) {
    	int len=(i%2==0)?size:0.618*size;
        glVertex2f(
            target.x + (0.5 * len * cos(2.0 * M_PI * 36*i/360)),
            target.y + (0.5 * len * sin(2.0 * M_PI * 36*i/360))
        );
    }
    glEnd();
}

void StarBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}






