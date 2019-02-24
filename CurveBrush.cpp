//
// LineBrush.cpp


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CurveBrush.h"
#include <math.h>
#include <numeric>
#include <stdlib.h>



extern float frand();

CurveBrush::CurveBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CurveBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void CurveBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	int size = pDoc->getSize();
	size *= 2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// These two lines do not work between glBegin() and glEnd()
	//New color in framebuffer =
	//	current alpha in framebuffer * current color in framebuffer +
	//	(1 - current alpha in framebuffer) * shader's output color
	glBegin(GL_LINE_STRIP);
	SetColor(source);

	glVertex2d(target.x, target.y);
	double leftx, lefty, rightx, righty, slope;
	slope = setGradientByOrigin(target.x, target.y);

	getNextPoint(target.x, target.y, leftx, lefty, slope, true);
	getNextPoint(target.x, target.y, rightx, righty, slope, true);

	glVertex2f(leftx, lefty); 
	glVertex2f(rightx, righty);

	double tempx, tempy;

	for (int i = 0; i < size / 2; i++) {
		slope = setGradientByOrigin(leftx, lefty);
		getNextPoint(leftx, lefty, tempx, tempy, slope, true);
		glVertex2f(tempx, tempy);
		leftx = tempx;
		lefty = tempy;
	}

	for (int i = 0; i < size / 2; i++) {
		slope = setGradientByOrigin(rightx, righty);
		getNextPoint(rightx, righty, tempx, tempy, slope, false);
		glVertex2f(tempx, tempy);
		rightx = tempx;
		righty = tempy;
	}

	glEnd();
}

void CurveBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

//this function is used to compute gradient and use it to set the angle of line brush
double CurveBrush::setGradientByOrigin(double x, double y) {
	ImpressionistDoc* pDoc = GetDocument();

	//In case source point is at edge
	if (x < 1) x = 1;
	else if (x > (pDoc->m_nWidth - 2)) x = pDoc->m_nWidth - 2;

	if (y < 1)y = 1;
	else if (y > pDoc->m_nHeight - 2)
		y = pDoc->m_nHeight - 2;

	GLubyte color[3];
	double grayscope[9];
	double grad_x_coef[9] = { -1,0,1, -2,0,2, -1,0,1 };
	double grad_y_coef[9] = { 1,2,1, 0,0,0, -1,-2,-1 };

	//calculate grayscope of surrounding pixels
	for (int i = 1; i > -2; i--) {
		for (int j = -1; j < 2; j++) {
			memcpy(color, pDoc->GetOriginalPixel((int)(x + j), (int)(y + i)), 3);
			grayscope[(1 - i) * 3 + j + 1] = (0.2126 *(GLdouble)color[0] + 0.7152 * (GLdouble)color[1] + 0.0722 * (GLdouble)color[2]) / 255;
		}
	}
	double x_grad, y_grad;
	x_grad = std::inner_product(std::begin(grayscope), std::end(grayscope), std::begin(grad_x_coef), 0.0);
	y_grad = std::inner_product(std::begin(grayscope), std::end(grayscope), std::begin(grad_y_coef), 0.0);

	return - x_grad / y_grad;
}

void CurveBrush::getNextPoint(double sourcex, double sourcey, double& resultx, double& resulty, double slope, bool left) {
	
	if (left) {
		resultx = sourcex - cos(atan(slope));
		resulty = sourcey - sin(atan(slope));
	}
	else {
		resultx = sourcex + cos(atan(slope));
		resulty = sourcey + sin(atan(slope));
	}
}