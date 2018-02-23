/*
 * StarBrush.h
 *
 *  Created on: 2018Äê2ÔÂ23ÈÕ
 *      Author: wyb
 */

#ifndef STARBRUSH_H_
#define STARBRUSH_H_

#include "ImpBrush.h"

class StarBrush : public ImpBrush {
public:
    StarBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

    void BrushBegin(const Point source, const Point target);
    void BrushMove(const Point source, const Point target);
    void BrushEnd(const Point source, const Point target);
    char* BrushName(void);


};


#endif /* STARBRUSH_H_ */
