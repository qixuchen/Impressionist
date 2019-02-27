// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "CircleBrush.h"
#include "LineBrush.h"
#include "ScatterPointBrush.h"
#include "ScatterLineBrush.h"
#include "ScatterCircleBrush.h"
#include "CurveBrush.h"
#include "EdgeBrush.h"


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	
	
	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_ucsave = NULL;
	m_ucGradient = NULL;
	m_ucAlpha = NULL;
	m_ucDissolve = NULL;

	m_nAngleType = ImpressionistUI::SLIDER_RIGHT_MOUSE;
	m_nAutoType = ImpressionistUI::automode::REGULAR;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatterPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatterLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatterCircleBrush( this, "Scattered Circles" );
	ImpBrush::c_pBrushes[BRUSH_CURVE_LINES]
		= new CurveBrush(this, "Curve Lines");

	m_pEdgeBrush = new EdgeBrush(this, "Edges");

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];
}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
	if (!strcmp(m_pCurrentBrush->BrushName(),"Lines") || !strcmp(m_pCurrentBrush->BrushName(), "Scattered Lines")) {// enable width, angle and angle control
		m_pUI->m_BrushWidthSlider->activate();
		m_pUI->m_BrushAngleSlider->activate();
		m_pUI->m_AngleTypeChoice->activate();
	}
	else {
		m_pUI->m_BrushWidthSlider->deactivate();
		m_pUI->m_BrushAngleSlider->deactivate();
		m_pUI->m_AngleTypeChoice->deactivate();
	}
}

void ImpressionistDoc::setAngleType(int type)
{
	m_nAngleType = type;
}

void ImpressionistDoc::setAutoType(int type)
{
	m_nAutoType = type;
}


//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}


//---------------------------------------------------------
// set the size of brush
//---------------------------------------------------------
void ImpressionistDoc::setSize(int size) {
	m_pUI->setSize(size);
}

//---------------------------------------------------------
// Returns the width
//---------------------------------------------------------
int ImpressionistDoc::getWidth()
{
	return m_pUI->getWidth();
}

void ImpressionistDoc::setWidth(int width)
{
	m_pUI->setWidth(width);
}

int ImpressionistDoc::getEdgeThreshold()
{
	return m_pUI->getEdgeThreshold();
}
//---------------------------------------------------------
// Returns the angle
//---------------------------------------------------------
int ImpressionistDoc::getAngle()
{
	return m_pUI->getAngle();
}


//---------------------------------------------------------
// set the angle
//---------------------------------------------------------
void ImpressionistDoc::setAngle(int angle) {
	m_pUI->setAngle(angle);
}




//---------------------------------------------------------
// Returns the alpha
//---------------------------------------------------------
float ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();
}


//---------------------------------------------------------
// Returns the dissolve alpha
//---------------------------------------------------------
float ImpressionistDoc::getDissolveAlpha()
{
	return m_pUI->getDissolveAlpha();
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname, bool mural) 
{
	if (this->m_pUI->m_origView->m_bImage == false && mural == true) {
		//MessageBox("No base picture has been loaded!\n");
		return 0;
	}
	// For whistle 3
	this->m_pUI->m_origView->m_bImage = true;

	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// Backup previous params.
	int prevWidth = -1;
	int prevHeight = -1;
	if (mural) {
		prevWidth = m_nWidth;
		prevHeight = m_nHeight;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;

	// Condition added for 1.5-point extra credit #2.
	if(!mural)
		if ( m_ucPainting ) delete [] m_ucPainting;

	// COndition added for dissolve image
	if (m_ucDissolve != NULL) m_pUI->m_applyDissolveButton->activate();

	m_ucBitmap		= data;

	// allocate space for draw view
	if (!mural) {
		m_ucPainting = new unsigned char[width*height * 3];
		memset(m_ucPainting, 0, width*height * 3);
		m_ucsave = new unsigned char[width*height * 3];
		memset(m_ucsave, 0, width*height * 3);
	}
	// For the mural effect. Extra credit features.
	else {
		int realSize = (width*height * 3 < prevWidth * prevHeight * 3) ? width * height * 3 : prevWidth * prevHeight * 3;

		unsigned char* temp1 = new unsigned char[width*height * 3];
		memset(temp1, 0, width*height * 3);
		for (int i = 0; i < (height < prevHeight ? height : prevHeight); i++) {
			memcpy(temp1 + 3 * i * width, m_ucPainting + 3 * i * prevWidth, (width < prevWidth) ? 3 * width : 3 * prevWidth);
		}
		delete[] m_ucPainting;
		m_ucPainting = temp1;

		unsigned char* temp2 = new unsigned char[width*height * 3];
		memset(temp2, 0, width*height * 3);
		for (int i = 0; i < (height < prevHeight ? height : prevHeight); i++) {
			memcpy(temp2 + 3 * i * width, m_ucsave + 3 * i * prevWidth, (width < prevWidth) ? 3 * width : 3 * prevWidth);
		}
		delete[] m_ucsave;
		m_ucsave = temp2;
	}

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}

//---------------------------------------------------------
// Load the specified image to represent gradient
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadGrad(char *iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nGradWidth = width;
	m_nGradHeight = height;

	// release old storage
	if (m_ucGradient) delete[] m_ucGradient;

	m_ucGradient = data;


	return 1;
}


//---------------------------------------------------------
// Load the specified image to represent Alpha
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadAlpha(char *iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width, height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nAlphaWidth = width;
	m_nAlphaHeight = height;

	// release old storage
	if (m_ucAlpha) delete[] m_ucAlpha;

	m_ucAlpha = data;


	return 1;
}



//---------------------------------------------------------
// Load the specified image to represent Alpha
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadDissolveAlpha(char *iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width, height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nDissolveAlphaWidth = width;
	m_nDissolveAlphaHeight = height;

	// release old storage
	if (m_ucDissolve) delete[] m_ucDissolve;

	m_ucDissolve = data;

	if(m_ucBitmap!=NULL)	m_pUI->m_applyDissolveButton->activate();
	return 1;
}

//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

GLubyte* ImpressionistDoc::GetPaintingPixel(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return (GLubyte*)(m_ucPainting + 3 * (y*m_nWidth + x));
}
//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

GLubyte* ImpressionistDoc::GetPaintingPixel(const Point p)
{
	return GetPaintingPixel(p.x, p.y);
}

//------------------------------------------------------------------
// Get pixel of the grad iamge
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetGradPixel(int x, int y)
{
	if (x%m_nGradWidth== (m_nGradWidth - 1)||x%m_nGradWidth==0)
		x = 1;
	else x = x%m_nGradWidth;

	if (y%m_nGradHeight==(m_nGradHeight - 1)||y%m_nGradHeight==0)
		y = 1;
	else y = y%m_nGradHeight;

	return (GLubyte*)(m_ucGradient + 3 * (y*m_nGradWidth + x));
}

//----------------------------------------------------------------
// Get pixel of the grad image
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetGradPixel(const Point p)
{
	return GetGradPixel(p.x, p.y);
}

//----------------------------------------------------------------
// Get pixel of the alpha image
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetAlphaPixel(const Point p)
{
	return (GLubyte*)(m_ucAlpha + 3 * (p.y*m_nAlphaWidth + p.x));
}


/*void ImpressionistDoc::swapOriginPaint() {
	unsigned char* temp = NULL;
	temp = m_ucBitmap;
	m_ucBitmap = m_ucPainting;
	m_ucPainting = temp;
	m_pUI->m_origView->refresh();
	printf("1");
	m_pUI->m_paintView->refresh();
}*/

//
void ImpressionistDoc::automaticPaint() {
	m_pUI->m_paintView->autoPaint();
}

void ImpressionistDoc::multiResPaint() {
	m_pUI->m_paintView->multiResPaint();
}

void ImpressionistDoc::edgePaint() {
	m_pUI->m_paintView->edgePaint();
}