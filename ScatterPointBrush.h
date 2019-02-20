#pragma once
#include "PointBrush.h"
class ScatterPointBrush :public PointBrush
{
public:
	ScatterPointBrush(ImpressionistDoc* pDoc, char* name);
	void BrushMove(const Point source, const Point target);
};

