//
// LineBrush.cpp


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"
#include <math.h>
#include <numeric>



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
		setGradientByOrigin(source);

	}
	else if (pDoc->m_nAngleType == ImpressionistUI::FOLLOW_ANOTHER_IMAGE) { // when angle control is the gradient of an IMAGE
		setGradientByImg(source);

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



//this function is used to compute gradient and use it to set the angle of line brush
void LineBrush::setGradientByOrigin(Point source) {
	ImpressionistDoc* pDoc = GetDocument();

	//In case source point is at edge
	if (source.x < 1) source.x = 1;
	else if (source.x > (pDoc->m_nWidth-2)) source.x = pDoc->m_nWidth - 2;

	if (source.y < 1)source.y = 1;
	else if (source.y > pDoc->m_nHeight-2)
		source.y = pDoc->m_nHeight - 2;
	
	GLubyte color[3];
	double grayscope[9];
	double grad_x_coef[9] = {-1,0,1, -2,0,2, -1,0,1};
	double grad_y_coef[9] = { 1,2,1, 0,0,0, -1,-2,-1 };

	//calculate grayscope of surrounding pixels
	for (int i = 1; i >-2; i--) {
		for (int j = -1; j < 2; j++) {
			memcpy(color, pDoc->GetOriginalPixel(source.x+j,source.y+i), 3);
			grayscope[(1-i) * 3 + j + 1] = (0.2126 *(GLdouble)color[0] + 0.7152 * (GLdouble)color[1] + 0.0722 * (GLdouble)color[2]) / 255;
		}
	}
	double x_grad, y_grad;
	x_grad = std::inner_product(std::begin(grayscope), std::end(grayscope), std::begin(grad_x_coef), 0.0);
	y_grad = std::inner_product(std::begin(grayscope), std::end(grayscope), std::begin(grad_y_coef), 0.0);
	pDoc->setAngle((int)(atan( y_grad/ x_grad) / M_PI * 180) + 90);

}


//this function is used to compute gradient when using another IMAGE as gradient controler
void LineBrush::setGradientByImg(Point source) {
	ImpressionistDoc* pDoc = GetDocument();

	GLubyte color[3];
	double grayscope[9];
	double grad_x_coef[9] = { -1,0,1, -2,0,2, -1,0,1 };
	double grad_y_coef[9] = { 1,2,1, 0,0,0, -1,-2,-1 };

	//calculate grayscope of surrounding pixels
	for (int i = 1; i > -2; i--) {
		for (int j = -1; j < 2; j++) {
			memcpy(color, pDoc->GetGradPixel(source.x + j, source.y + i), 3);
			grayscope[(1 - i) * 3 + j + 1] = (0.2126 *(GLdouble)color[0] + 0.7152 * (GLdouble)color[1] + 0.0722 * (GLdouble)color[2]) / 255;
		}
	}
	double x_grad, y_grad;
	x_grad = std::inner_product(std::begin(grayscope), std::end(grayscope), std::begin(grad_x_coef), 0.0);
	y_grad = std::inner_product(std::begin(grayscope), std::end(grayscope), std::begin(grad_y_coef), 0.0);
	pDoc->setAngle((int)(atan(y_grad / x_grad) / M_PI * 180) + 90);

}