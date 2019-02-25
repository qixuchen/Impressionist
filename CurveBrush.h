//
// LineBrush.h
//
// The header file for Line Brush. 
//

#ifndef CURVEBRUSH_H
#define CURVEBRUSH_H

#include "ImpBrush.h"

class CurveBrush : public ImpBrush
{
public:
	CurveBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	double setGradientByOrigin(double, double);
	char* BrushName(void);
	void getNextPoint(double sourcex, double sourcey, double& resultx, double& resulty, double slope, bool left);
};

#endif
