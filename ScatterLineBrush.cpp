#include "ScatterLineBrush.h"
#include "impressionistUI.h"
#include "impressionistDoc.h"
#include <math.h>


extern float frand();
ScatterLineBrush::ScatterLineBrush(ImpressionistDoc* pDoc, char* name):LineBrush(pDoc,name)
{
}


// scatter line brush
void ScatterLineBrush::BrushMove(const Point source, const Point target)
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
	float angle = pDoc->getAngle()* M_PI / 180;



	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// These two lines do not work between glBegin() and glEnd()
	//New color in framebuffer =
	//	current alpha in framebuffer * current color in framebuffer +
	//	(1 - current alpha in framebuffer) * shader's output color


	//first line
	drawLine(size, width, angle, source, target);

	//draw two more lines
	for (int i = 0; i < 2; i++) {
		int x_shift = 10 * frand() - 5;
		int y_shift = 10 * frand() - 5;
		Point new_source(source.x + x_shift, source.y + y_shift);
		Point new_target(target.x + x_shift, target.y + y_shift);
		drawLine(size, width, angle, new_source, new_target);
	}
}
