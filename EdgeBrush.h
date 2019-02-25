//
// EdgeBrush.h
//
// The header file for Edge Brush. 
//

#ifndef EDGEBRUSH_H
#define EDGEBRUSH_H

#include "ImpBrush.h"
#include <math.h>
#include <numeric>

class EdgeBrush : public ImpBrush
{
public:
	EdgeBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
	double getGradientNorm(Point source);
};

#endif
