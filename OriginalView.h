//
// originalview.h
//
// The header file for original view of the input images
//

#ifndef ORIGINALVIEW_H
#define ORIGINALVIEW_H

#include <FL/Fl.H>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>

#include "ImpBrush.h"

class ImpressionistDoc;

class OriginalView : public Fl_Gl_Window
{
public:
	OriginalView(int x, int y, int w, int h, const char *l);
	
	void draw();
	void refresh();

	void resizeWindow(int width, int height);

	void showSource(int x, int y);


	ImpressionistDoc*	m_pDoc;

	void SaveCurrentContent();

	void RestoreContent();

	void drawCursorMovement();

	void showPoint();


	int	isAnEvent;

	Point target;

	bool m_bImage; // Indicate whether a picture has been loaded or not.

private:
	int	m_nWindowWidth, 
		m_nWindowHeight;

	// Below are used for whistle 3.
	int drawWidth, drawHeight;


	GLvoid* bitstart; // Used for tracking cursor movement.
};

#endif