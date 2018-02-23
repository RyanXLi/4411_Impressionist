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

    void setMatrix(std::vector<std::vector<int>> m_matrix, int m_matrixDim) {
        matrix = m_matrix;
        matrixDim = m_matrixDim;
    }




    GLuint intensity(Point point) {
        ImpressionistDoc* pDoc = GetDocument();
        
        GLubyte red = (pDoc->GetOriginalPixel(point))[0];
        GLubyte green = (pDoc->GetOriginalPixel(point))[1];
        GLubyte blue = (pDoc->GetOriginalPixel(point))[2];
        return 0.299 * red + 0.587 * green + 0.114 * blue;
    }

    std::vector<std::vector<int>> matrix;
    int matrixDim = 0;


};

#endif
