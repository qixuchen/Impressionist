// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image
	int		loadGrad(char *iname);			// called by the UI to load  GRADIENT image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	void	setAngleType(int type);			// called by the UI to set the AngleType
	int		getSize();						// get the brush size
	void	setSize(int size);				// set the brush size
	int		getWidth();						// get the brush width 
	void	setWidth(int width);			// set the brush width
	int		getAngle();						// get the brush angle
	void	setAngle(int angle);			// set the brush angle
	float	getAlpha();						// get the brush alpha
	void	setAlpha(float alpha);			// set the brush alpha
	char*	getImageName();					// get the current image name
	
	void    swapOriginPaint();

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	

	//Dimensions of the gradient image window.
	int				m_nGradWidth,
					m_nGradHeight;


	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;
	// Bitmaps used for undo.
	unsigned char* m_ucsave;

	//Bitmaps for gradient.
	unsigned char* m_ucGradient;


	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	// Size of the brush.
	int m_nSize;
	int m_nAngleType;

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  
	// Get the color of the original picture at the specified coord
	GLubyte* GetGradPixel(int x, int y);
	// Get the color of the original picture at the specified point	
	GLubyte* GetGradPixel(const Point p);

private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
