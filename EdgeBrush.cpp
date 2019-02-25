//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "edgebrush.h"

extern float frand();

EdgeBrush::EdgeBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void EdgeBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void EdgeBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	int threshold = pDoc->getEdgeThreshold();
	if (getGradientNorm(source) <= threshold) {
		return;
	}

	glPointSize(1);
	glBegin(GL_POINTS);

	glColor4f(1, 1, 1, 1);

	glVertex2d(target.x, target.y);

	glEnd();
}

void EdgeBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

double EdgeBrush::getGradientNorm(Point source) {
	ImpressionistDoc* pDoc = GetDocument();

	//In case source point is at edge
	if (source.x < 1) source.x = 1;
	else if (source.x > (pDoc->m_nWidth - 2)) source.x = pDoc->m_nWidth - 2;

	if (source.y < 1)source.y = 1;
	else if (source.y > pDoc->m_nHeight - 2)
		source.y = pDoc->m_nHeight - 2;

	GLubyte color[3];
	double grayscope[9];
	double grad_x_coef[9] = { -1,0,1, -2,0,2, -1,0,1 };
	double grad_y_coef[9] = { 1,2,1, 0,0,0, -1,-2,-1 };

	//calculate grayscope of surrounding pixels
	for (int i = 1; i > -2; i--) {
		for (int j = -1; j < 2; j++) {
			memcpy(color, pDoc->GetOriginalPixel(source.x + j, source.y + i), 3);
			grayscope[(1 - i) * 3 + j + 1] = (0.2126 *(GLdouble)color[0] + 0.7152 * (GLdouble)color[1] + 0.0722 * (GLdouble)color[2]) / 255;
		}
	}
	double x_grad, y_grad;
	x_grad = std::inner_product(std::begin(grayscope), std::end(grayscope), std::begin(grad_x_coef), 0.0);
	y_grad = std::inner_product(std::begin(grayscope), std::end(grayscope), std::begin(grad_y_coef), 0.0);
	return (pow(x_grad, 2) + pow(y_grad, 2)) * 5; // 5 here is a tuned hyperparameter.
}