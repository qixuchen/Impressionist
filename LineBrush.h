//
// LineBrush.h
//
// The header file for Line Brush. 
//

#ifndef LINEBRUSH_H
#define LINEBRUSH_H

#include "ImpBrush.h"

class LineBrush : public ImpBrush
{
public:
	LineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void drawLine(int size, int width, float angle, const Point source, const Point target);
	void setGradient(Point source);
	char* BrushName(void);
protected:
	//these two point are used to catch the motion of mouse
	//used if angle control is mouse movement
	Point prev;
	Point cur;
};

#endif