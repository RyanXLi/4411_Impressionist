/*
 * CoilBrush.cpp
 *
 *  Created on: 2018Äê2ÔÂ23ÈÕ
 *      Author: wyb
 */


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CoilBrush.h"
#include <math.h>

extern float frand();



void drawCircle(int x,int y,float radius)
{
	float DEG2RAD = 3.14159/180;
	glBegin(GL_POINTS);

	for (int i=0; i< 360; i++){
      float degInRad = i*DEG2RAD;
      glVertex2f(x+cos(degInRad)*radius,y+sin(degInRad)*radius);
	}

    glEnd();
}

CoilBrush::CoilBrush(ImpressionistDoc* pDoc, char* name) :
    ImpBrush(pDoc, name) {
}


void CoilBrush::BrushBegin(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    pDoc->hasDrawn = 1;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BrushMove(source, target);
}

void CoilBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    if (pDoc == NULL) {
        printf("CircleBrush::BrushMove  document is NULL\n");
        return;
    }


    SetColorWithAlpha(source);

    int size = pDoc->getSize()+5;

    int loops = size/2;
    glVertex2f(target.x, target.y); // center
    for (int i = 0; i <= loops; i++) {
    	drawCircle(target.x,target.y,i*size/4);
    }
    glEnd();
}

void CoilBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}


