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
	
	if (pDoc->m_pUI->getColorControlMode() == false) {
		glColor4f((GLfloat)color[0] / 255, (GLfloat)color[1] / 255, (GLfloat)color[2] / 255, pDoc->getAlpha());
	}
	else {
		int red = pDoc->m_pUI->getRed();
		int green = pDoc->m_pUI->getGreen();
		int blue= pDoc->m_pUI->getBlue();
		glColor4f(((GLfloat)color[0]*0.3+(GLfloat)red*0.7)/ 255, ((GLfloat)color[1] * 0.3 + (GLfloat)green*0.7) / 255, ((GLfloat)color[2] * 0.3 + (GLfloat)blue*0.7) / 255, pDoc->getAlpha());
	}
}