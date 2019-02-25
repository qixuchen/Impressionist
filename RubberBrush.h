//
//

// Currently not used.
#ifndef RUBBERBRUSH_H
#define RUBBERBRUSH_H

#include "ImpBrush.h"

class RubberBrush : public ImpBrush
{
public:
	RubberBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
	void SetColorPixel(const Point source);
};

#endif#pragma once
