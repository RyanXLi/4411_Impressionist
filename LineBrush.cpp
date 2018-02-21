//
// LineBrush.cpp
//


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"
#include "math.h"

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
    firstClick=true;
    lastCoor.x=source.x;
    lastCoor.y=source.y;
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
    int angle=pDoc->getLineAngle();
    if (pDoc->m_pCurrentStrokeDirection == DIRECTION_BRUSH_DIRECTION){
    	if(firstClick){
    		firstClick=false;
    		glPopMatrix();
    		return;
    	}
    	else{
    		angle=(int)(atan2(source.y - lastCoor.y, source.x - lastCoor.x) * 180 / M_PI);
    		lastCoor.x=source.x;
    		lastCoor.y=source.y;

    	}
    }
    if (pDoc->m_pCurrentStrokeDirection == DIRECTION_GRADIENT){
    	ImpressionistDoc* doc=GetDocument();
    	int x1=doc->GetOriginalGreyscale(source.x,source.y);
    	int x2=doc->GetOriginalGreyscale(source.x-1,source.y);
    	int y1=doc->GetOriginalGreyscale(source.x,source.y);
    	int y2=doc->GetOriginalGreyscale(source.x,source.y-1);
    	angle=(int)(atan2(y1 - y2, x1-x2) * 180 / M_PI)+90;
    }
    glRotatef(angle, 0.0, 0.0, 1.0);

    glBegin(GL_LINES);

    glVertex2d(- size / 2, 0);
    glVertex2d(size / 2, 0);

    glEnd();
    glPopMatrix();
}

void LineBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}



