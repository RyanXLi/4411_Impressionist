/*
 * CoilBrush.h
 *
 *  Created on: 2018Äê2ÔÂ23ÈÕ
 *      Author: wyb
 */

#ifndef COILBRUSH_H_
#define COILBRUSH_H_


#include "ImpBrush.h"

class CoilBrush : public ImpBrush {
public:
	CoilBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

    void BrushBegin(const Point source, const Point target);
    void BrushMove(const Point source, const Point target);
    void BrushEnd(const Point source, const Point target);
    char* BrushName(void);


};


#endif /* COILBRUSH_H_ */
