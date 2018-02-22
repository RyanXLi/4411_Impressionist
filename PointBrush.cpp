//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "pointbrush.h"

extern float frand();

PointBrush::PointBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void PointBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

    pDoc->hasDrawn = 1;

	int size = pDoc->getSize();

	glPointSize( (float)size );

	BrushMove( source, target );
}

void PointBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

    //printf("POINTBRUSH: Source: (%d, %d); Target: (%d, %d).\n", source.x, source.y, target.x, target.y);

	if ( pDoc == NULL ) {
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}

	glBegin( GL_POINTS );
		SetColorWithAlpha( source );

		glVertex2d( target.x, target.y );

	glEnd();

    //printf("Now at %d, %d\n", target.x, target.y);
}

void PointBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

