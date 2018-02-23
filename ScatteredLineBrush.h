//
// ScatteredLineBrush
//
// The header file for ScatteredLineBrush
//

#ifndef SCATTEREDLINEBRUSH_H
#define SCATTEREDLINEBRUSH_H

#include "ImpBrush.h"

class ScatteredLineBrush : public ImpBrush {
public:
    ScatteredLineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

    void BrushBegin(const Point source, const Point target);
    void BrushMove(const Point source, const Point target);
    void BrushEnd(const Point source, const Point target);
    char* BrushName(void);

private:
    bool firstClick = FALSE;
    Point lastCoor = { 0, 0 };
};

#endif

