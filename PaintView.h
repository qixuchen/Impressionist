//
// paintview.h
//
// The header file for painting view of the input images
//

#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

class ImpressionistDoc;


extern int irand(int max);
extern float frand();

class PaintView : public Fl_Gl_Window
{
public:
	PaintView(int x, int y, int w, int h, const char* l);
	void draw();
	int handle(int event);

	void refresh();
	
	void resizeWindow(int width, int height);

	void SaveCurrentContent();

	void RestoreContent();

	void Undo();

	void SwapOrgPnt();

	void Dissolve();
	ImpressionistDoc *m_pDoc;

	// All these painting methods are implemented in the draw() function as an event_To_Do.

	void autoPaint();

	void loopPaint(int iIncrement, int jIncrement, bool diffCheck);

	void multiResPaint();

	void edgePaint();

	void clearBackground(float alpha);
	void addBackground(float alpha);


private:
	GLvoid* m_pPaintBitstart;
	GLvoid* m_pActualBitstart;
	int		m_nDrawWidth,
			m_nDrawHeight,
			m_nStartRow, 
			m_nEndRow,
			m_nStartCol, 
			m_nEndCol,
			m_nWindowWidth, 
			m_nWindowHeight;
	bool	m_bSwap, // Indicate whether a swap has been done.(to disable undo).
			m_bMove; // Indicate whether the mouse moves or not.

};

#endif