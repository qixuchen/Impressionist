#pragma once
#include "LineBrush.h"
class ScatterLineBrush :
	public LineBrush
{
public:
	ScatterLineBrush(ImpressionistDoc* pDoc,char* name);
	void BrushMove(const Point source, const Point target);
};

