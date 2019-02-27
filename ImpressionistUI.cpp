//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.h>

#include <math.h>

#include "impressionistUI.h"
#include "impressionistDoc.h"

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
    m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}

void ImpressionistUI::cb_load_mural_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadImage(newfile, true);
	}
}


//------------------------------------------------------------------
// load gradient image
//------------------------------------------------------------------
void ImpressionistUI::cb_load_grad(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadGrad(newfile);
	}
	whoami(o)->AngleTypeMenu[FOLLOW_ANOTHER_IMAGE].activate();
}


//------------------------------------------------------------------
// load alpha image
//------------------------------------------------------------------
void ImpressionistUI::cb_load_alpha(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadAlpha(newfile);
	}
}

//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp" );
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}


//-------------------------------------------------------------
// Brings up the color dialog
//-------------------------------------------------------------
void ImpressionistUI::cb_Color(Fl_Menu_* o, void* v)
{
	whoami(o)->m_colorDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();

}

//-----------------------------------------------------------
// BELL: Undo
//-----------------------------------------------------------
void ImpressionistUI::cb_undo(Fl_Menu_* o, void* v)
{
	whoami(o)->m_paintView->Undo();
}

//-----------------------------------------------------------
// BELL: Swap
//-----------------------------------------------------------
void ImpressionistUI::cb_swap(Fl_Menu_* o, void* v)
{
	whoami(o)->m_paintView->SwapOrgPnt();
}

//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version for CS341, Spring 2002");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	int type=(int)v;


	pDoc->setBrushType(type);
}



void ImpressionistUI::cb_angleChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;

	pDoc->setAngleType(type);
}

void ImpressionistUI::cb_autoChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;


	pDoc->setAutoType(type);
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}

// Self-explanatory
void ImpressionistUI::cb_auto_paint_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->automaticPaint();
}

// Self-explanatory
void ImpressionistUI::cb_multi_res_paint_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->multiResPaint();
}

// Self-explanatory
void ImpressionistUI::cb_edge_paint_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->edgePaint();
}

//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}


//-----------------------------------------------------------
// Updates the brush width
// Called by the UI when the width slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_widthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nWidth = int(((Fl_Slider *)o)->value());
}


//-----------------------------------------------------------
// Updates the brush angle
// Called by the UI when the angle slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_angleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAngle = int(((Fl_Slider *)o)->value());
}


//-----------------------------------------------------------
// Updates the brush alpha
// Called by the UI when the alpha slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAlpha = float(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_edgeThresholdSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nEdgeThreshold = int(((Fl_Slider *)o)->value());
}


//-----------------------------------------------------------
// Updates the red slide
// the following two updates the green and blue slides
//-----------------------------------------------------------
void ImpressionistUI::cb_redSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nRed = int(((Fl_Slider *)o)->value());
	((ImpressionistUI*)(o->user_data()))->m_nManualColorControl = true;
}

void ImpressionistUI::cb_greenSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nGreen = int(((Fl_Slider *)o)->value());
	((ImpressionistUI*)(o->user_data()))->m_nManualColorControl = true;
}

void ImpressionistUI::cb_blueSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nBlue = int(((Fl_Slider *)o)->value());
	((ImpressionistUI*)(o->user_data()))->m_nManualColorControl = true;
}


//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize=size;

	if (size<=40) 
		m_BrushSizeSlider->value(m_nSize);
}



//------------------------------------------------
// Return the brush width
//------------------------------------------------
int ImpressionistUI::getWidth()
{
	return m_nWidth;
}

int ImpressionistUI::getEdgeThreshold()
{
	return m_nEdgeThreshold;
}

//-------------------------------------------------
// Set the brush width
//-------------------------------------------------
void ImpressionistUI::setWidth(int width)
{
	m_nWidth = width;

	if (width <= 40)
		m_BrushWidthSlider->value(m_nWidth);
}

//------------------------------------------------
// Return the brush angle
//------------------------------------------------
int ImpressionistUI::getAngle()
{
	return m_nAngle;
}

//-------------------------------------------------
// Set the brush angle
//-------------------------------------------------
void ImpressionistUI::setAngle(int angle)
{
	m_nAngle = angle;

	if (angle <= 359)
		m_BrushAngleSlider->value(m_nAngle);
}

//------------------------------------------------
// Return the brush alpha
//------------------------------------------------
float ImpressionistUI::getAlpha()
{
	return m_nAlpha;
}

//-------------------------------------------------
// Set the brush alpha
//-------------------------------------------------
void ImpressionistUI::setAlpha(float alpha)
{
	m_nAlpha = alpha;

	if (alpha <= 1.0)
		m_BrushAlphaSlider->value(m_nAlpha);
}


bool ImpressionistUI::getColorControlMode()
{
	return m_nManualColorControl;
}

//------------------------------------------------
// Return R G B
//------------------------------------------------
int ImpressionistUI::getRed()
{
	return m_nRed;
}
int ImpressionistUI::getGreen()
{
	return m_nGreen;
}
int ImpressionistUI::getBlue()
{
	return m_nBlue;
}
// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Add Mural Image...",	FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_load_mural_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes }, 
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },
		{ "&Color",	FL_ALT + 'o', (Fl_Callback *)ImpressionistUI::cb_Color },
		{ "&Import Gradient Image",	FL_ALT + 'i', (Fl_Callback *)ImpressionistUI::cb_load_grad },
		{ "&Import Alpha map",	FL_ALT + 'i', (Fl_Callback *)ImpressionistUI::cb_load_alpha },
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },

	{ "&Option",		0, 0, 0, FL_SUBMENU },
		{ "&Undo",	FL_ALT + 'u', (Fl_Callback *)ImpressionistUI::cb_undo },
		{ "&Swap",	FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_swap },
		{ 0 },
	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {"Curve Lines",	FL_ALT + 'f', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CURVE_LINES},
  {0}
};


// Angle control menu definition
Fl_Menu_Item ImpressionistUI::AngleTypeMenu[] = {
  {"Slider/Right Mouse",		FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)SLIDER_RIGHT_MOUSE},
  {"Gradient",					FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)GRADIENT},
  {"Brush Direction",			FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)BRUSH_DIRECTION},
  {"Follow Image",			FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)FOLLOW_ANOTHER_IMAGE},
  {0}
};

// Angle control menu definition
Fl_Menu_Item ImpressionistUI::AutoTypeMenu[] = {
  {"Sequential",		FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_autoChoice, (void *)REGULAR},
  {"Random",					FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_autoChoice, (void *)RANDOM},
  {0}
};




//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
    m_mainWindow->end();

	// init values

	m_nSize = 10;
	m_nWidth = 1;
	m_nAngle = 0;
	m_nAlpha = 1.0;
	m_nEdgeThreshold = 5;
	m_nRed = m_nGreen = m_nBlue = 0;
	AngleTypeMenu[FOLLOW_ANOTHER_IMAGE].deactivate();

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 390, "Brush Dialog");
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		m_AngleTypeChoice = new Fl_Choice(112, 50, 150, 25, "&Stroke Direction");
		m_AngleTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_AngleTypeChoice->menu(AngleTypeMenu);
		m_AngleTypeChoice->callback(cb_angleChoice);
		m_AngleTypeChoice->deactivate();


		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 120, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);


		// Add line width slider to the dialog 
		m_BrushWidthSlider = new Fl_Value_Slider(10, 160, 300, 20, "Line Width");
		m_BrushWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushWidthSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushWidthSlider->labelfont(FL_COURIER);
		m_BrushWidthSlider->labelsize(12);
		m_BrushWidthSlider->minimum(1);
		m_BrushWidthSlider->maximum(40);
		m_BrushWidthSlider->step(1);
		m_BrushWidthSlider->value(m_nWidth);
		m_BrushWidthSlider->align(FL_ALIGN_RIGHT);
		m_BrushWidthSlider->callback(cb_widthSlides);
		m_BrushWidthSlider->deactivate();


		// Add brush angle slider to the dialog 
		m_BrushAngleSlider = new Fl_Value_Slider(10, 200, 300, 20, "Line Angle");
		m_BrushAngleSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushAngleSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAngleSlider->labelfont(FL_COURIER);
		m_BrushAngleSlider->labelsize(12);
		m_BrushAngleSlider->minimum(0);
		m_BrushAngleSlider->maximum(359);
		m_BrushAngleSlider->step(1);
		m_BrushAngleSlider->value(m_nAngle);
		m_BrushAngleSlider->align(FL_ALIGN_RIGHT);
		m_BrushAngleSlider->callback(cb_angleSlides);
		m_BrushAngleSlider->deactivate();

		// Add brush alpha slider to the dialog 
		m_BrushAlphaSlider = new Fl_Value_Slider(10, 240, 300, 20, "Alpha");
		m_BrushAlphaSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushAlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAlphaSlider->labelfont(FL_COURIER);
		m_BrushAlphaSlider->labelsize(12);
		m_BrushAlphaSlider->minimum(0.0);
		m_BrushAlphaSlider->maximum(1.0);
		m_BrushAlphaSlider->step(0.01);
		m_BrushAlphaSlider->value(m_nAlpha);
		m_BrushAlphaSlider->align(FL_ALIGN_RIGHT);
		m_BrushAlphaSlider->callback(cb_alphaSlides);

		m_autoPaintButton = new Fl_Button(10, 280, 150, 20, "&Auto Paint");
		m_autoPaintButton->user_data((void*)(this));
		m_autoPaintButton->callback(cb_auto_paint_button);

		m_AutoTypeChoice = new Fl_Choice(220, 280, 150, 20, "Style");
		m_AutoTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_AutoTypeChoice->menu(AutoTypeMenu);
		m_AutoTypeChoice->callback(cb_autoChoice);

		m_multiResPaintButton = new Fl_Button(10, 320, 150, 20, "&Multi Res Paint");
		m_multiResPaintButton->user_data((void*)(this));
		m_multiResPaintButton->callback(cb_multi_res_paint_button);

		m_EdgeThresholdSlider = new Fl_Value_Slider(10, 360, 150, 20, "Edge");
		m_EdgeThresholdSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_EdgeThresholdSlider->type(FL_HOR_NICE_SLIDER);
		m_EdgeThresholdSlider->labelfont(FL_COURIER);
		m_EdgeThresholdSlider->labelsize(12);
		m_EdgeThresholdSlider->minimum(1);
		m_EdgeThresholdSlider->maximum(20);
		m_EdgeThresholdSlider->step(1);
		m_EdgeThresholdSlider->value(m_nEdgeThreshold);
		m_EdgeThresholdSlider->align(FL_ALIGN_RIGHT);
		m_EdgeThresholdSlider->callback(cb_edgeThresholdSlides);

		m_edgePaintButton = new Fl_Button(220, 360, 150, 20, "&Edge Paint");
		m_edgePaintButton->user_data((void*)(this));
		m_edgePaintButton->callback(cb_edge_paint_button);

    m_brushDialog->end();	

	//for color control
	m_colorDialog = new Fl_Window(200, 200, "Color");
		m_redSlider = new Fl_Value_Slider(10, 50, 150, 20, "Red");
		m_redSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_redSlider->type(FL_HOR_NICE_SLIDER);
		m_redSlider->labelfont(FL_COURIER);
		m_redSlider->labelsize(12);
		m_redSlider->minimum(0);
		m_redSlider->maximum(255);
		m_redSlider->step(1);
		m_redSlider->value(m_nRed);
		m_redSlider->align(FL_ALIGN_RIGHT);
		m_redSlider->callback(cb_redSlides);

		m_greenSlider = new Fl_Value_Slider(10, 90, 150, 20, "Green");
		m_greenSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_greenSlider->type(FL_HOR_NICE_SLIDER);
		m_greenSlider->labelfont(FL_COURIER);
		m_greenSlider->labelsize(12);
		m_greenSlider->minimum(0);
		m_greenSlider->maximum(255);
		m_greenSlider->step(1);
		m_greenSlider->value(m_nGreen);
		m_greenSlider->align(FL_ALIGN_RIGHT);
		m_greenSlider->callback(cb_greenSlides);

		m_blueSlider = new Fl_Value_Slider(10, 130, 150, 20, "Blue");
		m_blueSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_blueSlider->type(FL_HOR_NICE_SLIDER);
		m_blueSlider->labelfont(FL_COURIER);
		m_blueSlider->labelsize(12);
		m_blueSlider->minimum(0);
		m_blueSlider->maximum(255);
		m_blueSlider->step(1);
		m_blueSlider->value(m_nBlue);
		m_blueSlider->align(FL_ALIGN_RIGHT);
		m_blueSlider->callback(cb_blueSlides);

	m_colorDialog->end();

}
