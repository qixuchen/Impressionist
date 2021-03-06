//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include <math.h>


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6
#define AUTO_DRAW			7
#define MULTI_RES			8
#define EDGE_PIC			9


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;
static Point    grad_start(0,0);
static Point    grad_end(0,0);


PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	m_bSwap = false;
}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_pActualBitstart = m_pDoc->m_ucActualMap +
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	// Whistle 3
	// Update the cursor movement.
	if (m_bMove) {
		Point target(coord.x, m_nWindowHeight - coord.y);
		m_pDoc->m_pUI->m_origView->isAnEvent = 1;
		m_pDoc->m_pUI->m_origView->refresh();

		m_pDoc->m_pUI->m_origView->isAnEvent = 2;
		m_pDoc->m_pUI->m_origView->target = target;
		m_pDoc->m_pUI->m_origView->refresh();
	}

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();
	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		//BELL: clip brush stroke

		if (coord.x < 0) {
			coord.x = 0;
		}
		else if (coord.x > m_nDrawWidth) {
			coord.x = m_nDrawWidth;
		}
		if (coord.y < 0) {
			coord.y = 0;
		}
		else if (coord.y > m_nDrawHeight) {
			coord.y = m_nDrawHeight;
		}
		//BELL: clip brush stroke

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );

		// Whistle 3
		// Use for the part RIGHT_MOUSE_UP
		// Stupid compiler!
		// Author: Arceus
		int tempAngle;

		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			break;
		case LEFT_MOUSE_DRAG:
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );

			m_bSwap = false;
			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			if (strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Lines") && strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Scattered Lines"))break;
			if (m_pDoc->m_nAngleType != ImpressionistUI::SLIDER_RIGHT_MOUSE)break;
			grad_start.x = coord.x;
			grad_start.y = m_nWindowHeight - coord.y;
			break;
		case RIGHT_MOUSE_DRAG:
			if (strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Lines") && strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Scattered Lines"))break;
			if (m_pDoc->m_nAngleType != ImpressionistUI::SLIDER_RIGHT_MOUSE)break;

			RestoreContent();
			grad_end.x = coord.x;
			grad_end.y = m_nWindowHeight - coord.y;
			glBegin(GL_LINES);
			glColor3f(1.0, 0.0, 0.0);
			glVertex2d(grad_start.x, grad_start.y);
			glVertex2d(grad_end.x, grad_end.y);
			glEnd();
			break;
		case RIGHT_MOUSE_UP:
			if (strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Lines") && strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Scattered Lines"))break;
			if (m_pDoc->m_nAngleType != ImpressionistUI::SLIDER_RIGHT_MOUSE)break;

			RestoreContent();
			grad_end.x = coord.x;
			grad_end.y = m_nWindowHeight - coord.y;
			// Change of implementation.
			// author: Arceus
			tempAngle = (int) (atan(1.0*(grad_end.y - grad_start.y) / (1.0*(grad_end.x - grad_start.x))) / M_PI * 180);

			// Alter the angles according to the quadrant.
			if ((grad_end.y < grad_start.y) && (tempAngle < 0)) {
				tempAngle = 360 + tempAngle;
			}
			else if ((grad_end.y < grad_start.y) && (tempAngle > 0)) {
				tempAngle = 180 + tempAngle;
			}
			else if ((grad_end.y > grad_start.y) && (tempAngle < 0)) {
				tempAngle = 180 + tempAngle;
			}
			else if ((grad_end.x < grad_start.x) && (tempAngle == 0)) {
				tempAngle = 180 + tempAngle;
			}
			m_pDoc->setAngle(tempAngle);
			// Change ends here.

			// The following two lines are codes before.

			//m_pDoc->setAngle((int)(atan(1.0*(grad_end.y - grad_start.y) / (1.0*(grad_end.x - grad_start.x))) / M_PI * 180)+180);
			m_pDoc->setSize((int)sqrt(pow((grad_end.x - grad_start.x), 2) + pow((grad_end.y - grad_start.y), 2)));

			break;

		// 1.5 points extra credits.
		case AUTO_DRAW:
			// Draw.
			m_bSwap = false;
			if (m_pDoc->m_nAutoType == ImpressionistUI::automode::REGULAR) {
				bool lastMove = false;
				int iIncrement = m_pDoc->getSize() - 1;
				int jIncrement = m_pDoc->getSize() - 1;
				if (!strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Lines")) {
					jIncrement = (int) (m_pDoc->getWidth() * 2);
					iIncrement /= 2;
				}
				if (!strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Circles") || !strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Scattered Circles") 
					|| !strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Scattered Lines")) {
					iIncrement /= 2;
					jIncrement /= 2;
				}
				loopPaint(iIncrement, jIncrement, false);
			}
			// Random paint.
			else {
				int originalSize = m_pDoc->getSize();
				int originalWidth = m_pDoc->getWidth();
				int originalAngle = m_pDoc->getAngle();
				int iterations = (int)((m_nDrawWidth / m_pDoc->getSize()) * (int)(m_nDrawHeight / m_pDoc->getSize()) * 1.5);
				for (int i = 0; i < iterations; i++) {
					int x = irand(m_nDrawWidth);
					int y = irand(m_nDrawHeight);
					Point currentPoint(x, y);
					source.x = currentPoint.x + m_nStartCol;
					source.y = m_nEndRow - currentPoint.y;
					target.x = currentPoint.x;
					target.y = m_nWindowHeight - currentPoint.y;
					if (i == 0) {
						m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
					}
					else if (i == iterations - 1) {
						m_pDoc->m_pCurrentBrush->BrushEnd(source, target);
						SaveCurrentContent();
						RestoreContent();
					}
					else {
						m_pDoc->m_pCurrentBrush->BrushMove(source, target);
					}
					
					// Randomly change size, angle, width.
					int size = originalSize + (int)(6 * frand() - 3);
					if (size < 3) {
						size = 3;
					}
					m_pDoc->setSize(size);
					
					if (!(strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Lines") &&
						strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Scattered Lines"))) {

						// Change width.
						
						int width = originalWidth + (int)(4 * frand() - 2);
						if (width < 1) {
							width = 1;
						}
						m_pDoc->setWidth(width);

						// Change angle.
				
						int angle = originalAngle + (int) (10 * frand() - 5);

						if (angle > 359) {
							angle = 359;
						}
						else if (angle < 0) {
							angle = 0;
						}
						m_pDoc->setAngle(angle);
					}

					glFlush();
				}
			}
			
			break;

		// Extra Credit.
		case MULTI_RES: {
			m_bSwap = false;
			
			//m_pDoc->setBrushType(BRUSH_POINTS);
			int granularity = (m_nDrawHeight > m_nDrawWidth ? m_nDrawWidth : m_nDrawHeight) / 10;;
			m_pDoc->setSize(granularity);
			int iIncrement = granularity;
			int jIncrement = granularity;
			loopPaint(iIncrement, jIncrement, false);

			
			while(granularity > 4) {
				granularity /= 2;
				m_pDoc->setSize(granularity);
				iIncrement = granularity;
				jIncrement = granularity;
				loopPaint(iIncrement, jIncrement, true);
			}
			
			
			break;
		}

		// Part of the extra credit.
		case EDGE_PIC:
			for (int i = 0; i < m_nDrawWidth; i++) {
				for (int j = 0; j < m_nDrawHeight; j++) {
					Point currentPoint(i, j);
					Point source(0, 0), target(0, 0);
					ImpBrush* temp = m_pDoc->m_pCurrentBrush;
					m_pDoc->m_pCurrentBrush = m_pDoc->m_pEdgeBrush;
					source.x = (currentPoint.x > m_nDrawWidth ? m_nDrawWidth : (currentPoint.x > 0 ? currentPoint.x : 0)) + m_nStartCol;
					source.y = m_nEndRow - (currentPoint.y > m_nDrawHeight ? m_nDrawHeight : (currentPoint.y > 0 ? currentPoint.y : 0));
					target.x = currentPoint.x;
					target.y = m_nWindowHeight - currentPoint.y;
					m_pDoc->m_pCurrentBrush->BrushMove(source, target);
					m_pDoc->m_pCurrentBrush = temp;
				}
			}
			SaveCurrentContent();
			RestoreContent();
			break;

		default:
			printf("Unknown event!!\n");		
			break;
		}
	}



	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA
}


int PaintView::handle(int event)
{
	switch (event)
	{
	case FL_ENTER:
		redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button() > 1)
			eventToDo = RIGHT_MOUSE_DOWN;
		else
			eventToDo = LEFT_MOUSE_DOWN;
		isAnEvent = 1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button() > 1)
			eventToDo = RIGHT_MOUSE_DRAG;
		else
			eventToDo = LEFT_MOUSE_DRAG;
		isAnEvent = 1;
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button() > 1)
			eventToDo = RIGHT_MOUSE_UP;
		else
			eventToDo = LEFT_MOUSE_UP;
		isAnEvent = 1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		// Whistle 3
		// Update the cursor movement.
		m_bMove = true;
		redraw();

		break;
	default:
		return 0;
		break;

	}

	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	memcpy(m_pDoc->m_ucsave, m_pDoc->m_ucPainting, m_pDoc->m_nPaintWidth*m_pDoc->m_nPaintHeight*3);
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );

	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

	glDrawBuffer(GL_FRONT_AND_BACK);
}

//BELL : UNDO
void PaintView::Undo() {

	// Add a boolean checking.
	if (!m_bSwap&&m_pDoc->m_ucsave!=NULL) {
		memcpy(m_pDoc->m_ucPainting, m_pDoc->m_ucsave, m_pDoc->m_nPaintWidth*m_pDoc->m_nPaintHeight * 3);
		glDrawBuffer(GL_FRONT_AND_BACK);
		redraw();
		glFlush();
	}
}


//BELL: Swap originView and PaintView 
void PaintView::SwapOrgPnt() {
	unsigned char* temp;

	// Add a boolean variable.
	// If detailed explanation is needed, please ask me directly.
	// Too lazy to write comments.
	m_bSwap = true;

	temp = m_pDoc->m_ucPainting;
	m_pDoc->m_ucPainting = m_pDoc->m_ucBitmap;
	m_pDoc->m_ucBitmap = temp;
	redraw();
	m_pDoc->m_pUI->m_origView->redraw();
	glFlush();

}

void PaintView::loopPaint(int iIncrement, int jIncrement, bool diffCheck) {
	for (int i = (int)(-iIncrement); i < m_nDrawWidth + iIncrement; i += iIncrement) {
		for (int j = (int)(-jIncrement); j < m_nDrawHeight + jIncrement; j += jIncrement) {
			Point currentPoint(i, j);
			Point source(0, 0), target(0, 0);
			source.x = (currentPoint.x > m_nDrawWidth ? m_nDrawWidth : (currentPoint.x > 0 ? currentPoint.x : 0)) + m_nStartCol;
			source.y = m_nEndRow - (currentPoint.y > m_nDrawHeight ? m_nDrawHeight : (currentPoint.y > 0 ? currentPoint.y : 0));
			target.x = currentPoint.x;
			target.y = m_nWindowHeight - currentPoint.y;
			if (diffCheck) {
				double diff = m_pDoc->m_pCurrentBrush->calDiff(source, target);
				if (diff < 0.15) {
					continue;
				}
			}
			if (i == 0 && j == 0) {
				m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
			}
			else if (i + iIncrement > m_nDrawWidth + iIncrement - 1 && j + jIncrement > m_nDrawHeight + jIncrement - 1) {
				m_pDoc->m_pCurrentBrush->BrushMove(source, target);
				m_pDoc->m_pCurrentBrush->BrushEnd(source, target);
			}
			else {
				m_pDoc->m_pCurrentBrush->BrushMove(source, target);
			}
			glFlush();
		}
	}
	SaveCurrentContent();
	RestoreContent();
}

void PaintView::autoPaint() {
	if (m_pDoc->m_pUI->m_origView->m_bImage) {
		isAnEvent = 1;
		eventToDo = AUTO_DRAW;
		redraw();
	}

}

void PaintView::multiResPaint() {
	// If a pic has been loaded.
	if (m_pDoc->m_pUI->m_origView->m_bImage) {
		isAnEvent = 1;
		eventToDo = MULTI_RES;
		redraw();
	}
}

void PaintView::edgePaint() {
	// If a pic has been loaded.
	if (m_pDoc->m_pUI->m_origView->m_bImage) {
		isAnEvent = 1;
		eventToDo = EDGE_PIC;
		redraw();
	}
}

void PaintView::Dissolve() {
	if (m_pDoc->m_ucDissolve != NULL) {
		int dw = m_pDoc->m_nDissolveAlphaWidth;
		int dh = m_pDoc->m_nDissolveAlphaHeight;
		int pw = m_pDoc->m_nPaintWidth;
		int ph = m_pDoc->m_nPaintHeight;
		GLubyte dImg[3]; //the pixel of dissolve image
		float alpha = m_pDoc->getDissolveAlpha();
		
		for (int i = 0; i < ph; i++) {
			for (int j = 0; j < pw; j++) {
				GLubyte* pixel= m_pDoc->m_ucPainting + 3 * (i*pw + j);
				int dx = j * 1.0 / pw * dw;
				int dy = i * 1.0 / ph * dh;
				memcpy(dImg, m_pDoc->m_ucDissolve + 3 * (dy*dw + dx), 3);
				
				*pixel = (int)((1 - alpha)*(*pixel) + (alpha)*dImg[0]);
				*(pixel+1) = (int)((1 - alpha)*(*(pixel+1)) + (alpha)*dImg[1]);
				*(pixel+2) = (int)((1 - alpha)*(*(pixel+2)) + (alpha)*dImg[2]);
			}
		}
		//SaveCurrentContent();
		//RestoreContent();
		refresh();
	}
}

void PaintView::clearBackground(float alpha) {
	
	for (int i = 0; i < m_nDrawHeight; i++) {
		for(int j=0; j<m_nDrawWidth;j++){
			int offset = 3 * (i*m_pDoc->m_nWidth + j);
			GLubyte dImg[3];
			memcpy(dImg, m_pDoc->m_ucBitmap +offset, 3);
			GLubyte* pixel = m_pDoc->m_ucPainting + offset;
			for (int p = 0; p < 3; p++) {
				if ((*(pixel + p) - (int)((alpha)*dImg[p] / 2)) < 0) *(pixel + p) = 0;
				else *(pixel + p) -= (int)((alpha)*dImg[p] / 2);
			}
		}
	}
}

void PaintView::addBackground(float alpha) {

	for (int i = 0; i < m_nDrawHeight; i++) {
		for (int j = 0; j < m_nDrawWidth; j++) {
			int offset = 3 * (i*m_pDoc->m_nWidth + j);
			GLubyte dImg[3];
			memcpy(dImg, m_pDoc->m_ucBitmap + offset, 3);
			GLubyte* pixel = m_pDoc->m_ucPainting + offset;
			for (int p = 0; p < 3; p++) {
				if ((*(pixel + p) + (int)((alpha)*dImg[p] / 2)) > 255) *(pixel + p) = 255;
				else *(pixel + p) += (int)((alpha)*dImg[p] / 2);
			}
		}
	}
}


/*
for (int i = 0; i < m_nDrawHeight; i++) {
	for(int j=0; j<m_nDrawWidth;j++){
		int offset = 3 * (i*m_pDoc->m_nWidth + j);
		GLubyte dImg[3];
		GLubyte p[3];
		memcpy(p, m_pDoc->m_ucPainting + offset, 3);
		memcpy(dImg, m_pDoc->m_ucBitmap +offset, 3);
		GLubyte* pixel = m_pDoc->m_ucActualMap + offset;
		*pixel = (int)(((1 - 0.2)*p[0]) + (0.2)*dImg[0]);
		*(pixel + 1) = (int)(((1 - 0.2)*p[1]) + (0.2)*dImg[1]);
		*(pixel + 2) = (int)(((1 - 0.2)*p[2]) + (0.2)*dImg[2]);
	}
}*/