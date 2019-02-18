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
	glPointSize((float)size);


	//first brush
	glBegin(GL_POINTS);
	SetColor(source);
	glVertex2d(target.x, target.y);

	glEnd();

	//randomly generate two more brush
	for (int i = 0; i < 2; i++) {
		int x_shift = 10 * frand()-5;
		int y_shift = 10 * frand()-5;
		Point new_source(source.x + x_shift, source.y + y_shift);
		glBegin(GL_POINTS);
		SetColor(new_source);
		glVertex2d(target.x + x_shift, target.y + y_shift);
		glEnd();
	}
}