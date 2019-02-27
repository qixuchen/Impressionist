//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "originalview.h"

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#endif


OriginalView::OriginalView(int			x, 
						   int			y, 
						   int			w, 
						   int			h, 
						   const char*	l)
							: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	isAnEvent = 0;
	m_bImage = false;
}

void OriginalView::draw()
{
#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA
	// Whistle 3
	// Drawing for the cursor movement is updated here.

	if (!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable(GL_DEPTH_TEST);

		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer(GL_FRONT);
		ortho();

	}
	if (isAnEvent && valid()) {
		if (m_bImage) {
			if (isAnEvent == 1) {
				SaveCurrentContent();
			}
			// The mouse is dragging.
			else {
				showPoint();
			}
		}
		isAnEvent = 0;
	}
	else {
		
		glClear(GL_COLOR_BUFFER_BIT);

		if (m_pDoc->m_ucBitmap)
		{
			// note that both OpenGL pixel storage and the Windows BMP format
			// store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
			// around with startrow.

			m_nWindowWidth = w();
			m_nWindowHeight = h();



			// we are not using a scrollable window, so ignore it
			Point scrollpos;	// = GetScrollPosition();
			scrollpos.x = scrollpos.y = 0;

			drawWidth = min(m_nWindowWidth, m_pDoc->m_nWidth);
			drawHeight = min(m_nWindowHeight, m_pDoc->m_nHeight);

			int	startrow = m_pDoc->m_nHeight - (scrollpos.y + drawHeight);
			if (startrow < 0)
				startrow = 0;


			bitstart = m_pDoc->m_ucBitmap + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);

			// just copy image to GLwindow conceptually
			glRasterPos2i(0, m_nWindowHeight - drawHeight);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth);
			glDrawBuffer(GL_BACK);
			glDrawPixels(drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart);

		}
	}
	
	glFlush();
	#ifndef MESA
		// To avoid flicker on some machines.
		glDrawBuffer(GL_BACK);
	#endif // !MESA
}



void OriginalView::refresh()
{
	redraw();
}

void OriginalView::resizeWindow(int	width, 
								int	height)
{
	resize(x(), y(), width, height);
}

void OriginalView::showSource(int x, int y) {

	glPointSize(50);
	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2d(100, 100);
	
	glEnd();
	glFlush();
	redraw();
}

// Below are all used for whistle 3
void OriginalView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);

	glReadPixels(0,
		m_nWindowHeight - drawHeight,
		drawWidth,
		drawHeight,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		bitstart);
}


void OriginalView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2i(0, m_nWindowHeight - drawHeight);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
	glDrawPixels(drawWidth,
		drawHeight,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		bitstart);

	//	glDrawBuffer(GL_FRONT);
}

void OriginalView::drawCursorMovement() {
	SaveCurrentContent();
	showPoint();
}

void OriginalView::showPoint() {
	RestoreContent();
	glPointSize(4);

	glBegin(GL_POINTS);

	glColor3d(255, 0, 0);

	glVertex2d(target.x, target.y);

	glEnd();
}