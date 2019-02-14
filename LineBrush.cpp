//
// LineBrush.cpp


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"
#include <math.h>

extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	int width = pDoc->getWidth();
	float angle = pDoc->getAngle()* M_PI/180;


	glBegin(GL_POLYGON);
	SetColor(source);
	//draw small polygon to represent line
	glVertex2d(target.x + cos(angle)*size / 2 - sin(angle)*width / 2, target.y + sin(angle)*size / 2 + cos(angle)*width / 2);
	glVertex2d(target.x + cos(angle)*size / 2 + sin(angle)*width / 2, target.y + sin(angle)*size / 2 - cos(angle)*width / 2);
	glVertex2d(target.x - cos(angle)*size / 2 + sin(angle)*width / 2, target.y - sin(angle)*size / 2 - cos(angle)*width / 2);
	glVertex2d(target.x - cos(angle)*size / 2 - sin(angle)*width / 2, target.y - sin(angle)*size / 2 + cos(angle)*width / 2);
	glEnd();
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
