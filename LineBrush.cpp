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
	prev = Point(0, 0);
	cur = Point(0, 0);
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	prev.x = target.x;
	prev.y = target.y;
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
	//if angle control is mouse movement
	if (pDoc->m_nAngleType == ImpressionistUI::BRUSH_DIRECTION) {
		cur.x = target.x;
		cur.y = target.y;
		if (cur.x != prev.x || cur.y != prev.y) {
			pDoc->setAngle((int)(atan(1.0*(cur.y - prev.y) / (1.0*(cur.x - prev.x))) / M_PI * 180) + 180);
		}
		prev.x = cur.x;
		prev.y = cur.y;
	}
	else if (pDoc->m_nAngleType == ImpressionistUI::GRADIENT) { // when angle control is the gradient
		pDoc->setAngle(computeGradient(source));

	}


	int size = pDoc->getSize();
	int width = pDoc->getWidth();
	float angle = pDoc->getAngle()* M_PI/180;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// These two lines do not work between glBegin() and glEnd()
	//New color in framebuffer =
	//	current alpha in framebuffer * current color in framebuffer +
	//	(1 - current alpha in framebuffer) * shader's output color
	drawLine(size, width, angle, source, target);
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

void LineBrush::drawLine(int size,int width, float angle, const Point source, const Point target) {
	glBegin(GL_POLYGON);
	SetColor(source);
	//draw small polygon to represent line
	glVertex2d(target.x + cos(angle)*size / 2 - sin(angle)*width / 2, target.y + sin(angle)*size / 2 + cos(angle)*width / 2);
	glVertex2d(target.x + cos(angle)*size / 2 + sin(angle)*width / 2, target.y + sin(angle)*size / 2 - cos(angle)*width / 2);
	glVertex2d(target.x - cos(angle)*size / 2 + sin(angle)*width / 2, target.y - sin(angle)*size / 2 - cos(angle)*width / 2);
	glVertex2d(target.x - cos(angle)*size / 2 - sin(angle)*width / 2, target.y - sin(angle)*size / 2 + cos(angle)*width / 2);
	glEnd();
}

int LineBrush::computeGradient(const Point source) {
	return 0;
}