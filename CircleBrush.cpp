//
// CircleBrush.cpp


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "Circlebrush.h"
#include <math.h>

extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();


	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	int size = pDoc->getSize();
	drawCircle(size,source,target);


}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

void CircleBrush::drawCircle(int size, const Point source, const Point target)
{

	double angle = 2 * 3.14159 / 100;
	glPolygonMode(GL_FRONT, GL_FILL);

	glBegin(GL_POLYGON);
	SetColor(source);
	double angle1 = 0.0;
	glVertex2d( target.x+size/2* cos(0.0), target.y+size/2 * sin(0.0));

	int i;
	for (i = 0; i < 100; i++)
	{
		glVertex2d(target.x+size/2 * cos(angle1), target.y+size/2 *sin(angle1));
		angle1 += angle;
	}

	glEnd();
}