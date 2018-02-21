//
// FilterBrush
//
// The header file for FilterBrush
//

#ifndef FILTERBRUSH_H
#define FILTERBRUSH_H

#include "ImpBrush.h"
#include <vector>

class FilterBrush : public ImpBrush {
public:
    FilterBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

    void BrushBegin(const Point source, const Point target);
    void BrushMove(const Point source, const Point target);
    void BrushEnd(const Point source, const Point target);
    char* BrushName(void);

    std::vector<std::vector<int>> matrix;
    int matrixDim = 0;


};

#endif
