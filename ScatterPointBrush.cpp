#include "ScatterPointBrush.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
extern float frand();


ScatterPointBrush::ScatterPointBrush(ImpressionistDoc* pDoc,char* name):PointBrush(pDoc,name)
{
}

void ScatterPointBrush::BrushMove(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// These two lines do not work between glBegin() and glEnd()
	//New color in framebuffer =
	//	current alpha in framebuffer * current color in framebuffer +
	//	(1 - current alpha in framebuffer) * shader's output color

	int size = pDoc->getSize();

	// Hardcode size to be a rather small number.
	glPointSize(1);
	// Previous implementation.
	//glPointSize((float)size);


	//first brush
	glBegin(GL_POINTS);
	SetColor(source);
	glVertex2d(target.x, target.y);

	glEnd();

	// Change implementation here:
	//		1. the area of drawing.
	//		2. # of points.
	//randomly generate two more brush
	for (int i = 0; i < size * size / 5; i++) {
		int x_shift = (int) (size * frand() - size / 2);
		int y_shift = (int)(size * frand() - size / 2);
		Point new_source(source.x + x_shift, source.y + y_shift);
		glBegin(GL_POINTS);
		SetColor(new_source);
		glVertex2d(target.x + x_shift, target.y + y_shift);
		glEnd();
	}
}