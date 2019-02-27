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
	float alpha = SetPaintAlpha(source);
	
	if (pDoc->m_pUI->getColorControlMode() == false) {
		glColor4f((GLfloat)color[0] / 255, (GLfloat)color[1] / 255, (GLfloat)color[2] / 255, alpha);
	}
	else {
		int red = pDoc->m_pUI->getRed();
		int green = pDoc->m_pUI->getGreen();
		int blue= pDoc->m_pUI->getBlue();
		glColor4f(((GLfloat)color[0]*0.3+(GLfloat)red*0.7)/ 255, ((GLfloat)color[1] * 0.3 + (GLfloat)green*0.7) / 255, ((GLfloat)color[2] * 0.3 + (GLfloat)blue*0.7) / 255, alpha);
	}
}

double ImpBrush::calDiff(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();

	GLubyte color1[3], color2[3];
	memcpy(color1, pDoc->GetOriginalPixel(source), 3);
	memcpy(color2, pDoc->GetPaintingPixel(source), 3);

	double totalScore = sqrt(3) * 255;
	double curScore = sqrt(pow(color1[0] - color2[0], 2) + pow(color1[1] - color2[1], 2)
		+ pow(color1[2] - color2[2], 2));
	return curScore / totalScore;
}

float ImpBrush::SetPaintAlpha(const Point source) {
	ImpressionistDoc* pDoc = GetDocument();
	if (pDoc->m_ucAlpha == NULL) return pDoc->getAlpha();
	else {
		Point alphaSource(0,0); // source point that set the alpha
		alphaSource.x = (int)((source.x*1.0 / pDoc->m_nWidth) *pDoc->m_nAlphaWidth);
		alphaSource.y = (int)((source.y*1.0 / pDoc->m_nHeight) *pDoc->m_nAlphaHeight);
		GLubyte alphaPixel[3];
		memcpy(alphaPixel, pDoc->GetAlphaPixel(alphaSource), 3);
		return (float)(0.2126 *(GLdouble)alphaPixel[0] + 0.7152 * (GLdouble)alphaPixel[1] + 0.0722 * (GLdouble)alphaPixel[2]) / 255;

	}
}