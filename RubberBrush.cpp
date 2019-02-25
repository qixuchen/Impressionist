//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "RubberBrush.h"

extern float frand();

RubberBrush::RubberBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void RubberBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void RubberBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	//glPointSize((float)size);


	glBegin(GL_POINTS);
	SetColor(source);

	glVertex2d(target.x, target.y);

	glEnd();
}

void RubberBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
void RubberBrush::SetColorPixel(const Point source)

{
	ImpressionistDoc* pDoc = GetDocument();

	GLubyte color[3];

	memcpy(color, pDoc->GetPaintingPixel(source), 3);

	glColor4f((GLfloat)color[0] / 255, (GLfloat)color[1] / 255, (GLfloat)color[2] / 255, pDoc->getAlpha());
}
