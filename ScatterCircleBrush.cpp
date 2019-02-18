#include "ScatterCircleBrush.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

extern float frand();
ScatterCircleBrush::ScatterCircleBrush(ImpressionistDoc* pDoc,char* name):CircleBrush(pDoc,name)
{
}

void ScatterCircleBrush::BrushMove(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// These two lines do not work between glBegin() and glEnd()
	//New color in framebuffer =
	//	current alpha in framebuffer * current color in framebuffer +
	//	(1 - current alpha in framebuffer) * shader's output color


	//draw the original circle
	drawCircle((float)size, source, target);

	//draw two new circles
	for (int i = 0; i < 2; i++) {
		int x_shift = 10 * frand() - 5;
		int y_shift = 10 * frand() - 5;
		Point new_source(source.x + x_shift, source.y + y_shift);
		Point new_target(target.x + x_shift, target.y + y_shift);
		drawCircle(size, new_source, new_target);
	}
}
