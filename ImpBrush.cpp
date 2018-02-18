//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();


	GLubyte color[3];

	memcpy ( color, pDoc->GetOriginalPixel( source ), 3 );
 
	glColor3ubv( color );

}



//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColorWithAlpha(const Point source) {
    ImpressionistDoc* pDoc = GetDocument();


    GLubyte color[4];

    memcpy(color, pDoc->GetOriginalPixel(source), 3);

    GLubyte glubyteMax = (GLubyte)0 - (GLubyte)1;
    GLubyte alpha = pDoc->getAlpha() * glubyteMax;
    color[3] = alpha;

    //printf("%u, ", color[0]);
    //printf("%u, ", color[1]);
    //printf("%u, ", color[2]);
    //printf("%u, ", color[3]);
    //printf("\n");

    glClear(GL_COLOR_BUFFER_BIT);

    glColor4ubv(color);

}
