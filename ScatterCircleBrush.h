#pragma once
#include "CircleBrush.h"

class ScatterCircleBrush :
	public CircleBrush
{
public:
	ScatterCircleBrush(ImpressionistDoc* pDoc,char* name);
	void BrushMove(const Point source, const Point target);
};

