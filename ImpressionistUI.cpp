//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.h>

#include <math.h>
#include <algorithm>

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
    pDoc->hasDrawn = 0;

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}


void ImpressionistUI::cb_load_other_image(Fl_Menu_* o, void* v) {
    ImpressionistDoc *pDoc = whoami(o)->getDocument();

    char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
    if (newfile != NULL) {
        pDoc->loadAnotherImage(newfile);
    }
}

void ImpressionistUI::cb_load_edge_image(Fl_Menu_* o, void* v) {
    ImpressionistDoc *pDoc = whoami(o)->getDocument();

    char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
    if (newfile != NULL) {
        pDoc->loadEdgeImage(newfile);
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

void ImpressionistUI::cb_color_dialog(Fl_Menu_* o, void* v) {
    whoami(o)->m_colorDialog->show();
}

void ImpressionistUI::cb_undo(Fl_Menu_* o, void* v){
	whoami(o)->m_paintView->needToUndo=true;
	whoami(o)->m_paintView->redraw();
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


void ImpressionistUI::cb_disolve(Fl_Menu_* o, void* v) {
    ImpressionistDoc* pDoc = whoami(o)->getDocument();
    pDoc->disolve();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();
    whoami(o)->m_colorDialog->hide();
}

void ImpressionistUI::cb_exchange_content(Fl_Menu_* o, void* v) {
    ImpressionistDoc* pDoc = whoami(o)->getDocument();
    
    pDoc->m_pUI->m_paintView->needToExchange = 1 - pDoc->m_pUI->m_paintView->needToExchange;
    pDoc->m_pUI->m_paintView->redraw();

    pDoc->m_pUI->m_origView->needToExchange = 1 - pDoc->m_pUI->m_origView->needToExchange;
    pDoc->m_pUI->m_origView->redraw();

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

    // printf("cb_brushChoice called with type %d", type);
}

void ImpressionistUI::cbDisplayChoice(Fl_Menu_* o, void* v)     {

    ImpressionistDoc *pDoc = whoami(o)->getDocument();

    int type = (int)v;
    if (type == DISPLAY_OTHER && !pDoc->otherImageLoaded) {
        fl_alert("other image not loaded");
        return;
    }

    pDoc->m_pUI->m_origView->displayImage = type;
    pDoc->m_pUI->m_origView->redraw();
}


void ImpressionistUI::cb_strokeDirectionChoice(Fl_Widget* o, void* v) {
    ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
    ImpressionistDoc* pDoc = pUI->getDocument();

    int type = (int)v;
    pDoc->setStrokeDirection(type);
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


//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}


void ImpressionistUI::cb_lineWidthSlides(Fl_Widget* o, void* v) 
{
    ((ImpressionistUI*)(o->user_data()))->m_lineWidth = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_lineAngleSlides(Fl_Widget* o, void* v) {
    ((ImpressionistUI*)(o->user_data()))->m_lineAngle = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v) {
    ((ImpressionistUI*)(o->user_data()))->m_alpha = double(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_spacingSlides(Fl_Widget* o, void* v) {
    ((ImpressionistUI*)(o->user_data()))->m_spacing = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_redSlides(Fl_Widget* o, void* v) {
    ((ImpressionistUI*)(o->user_data()))->m_red = double(((Fl_Slider *)o)->value());
}
void ImpressionistUI::cb_greenSlides(Fl_Widget* o, void* v) {
    ((ImpressionistUI*)(o->user_data()))->m_green = double(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_blueSlides(Fl_Widget* o, void* v) {
    ((ImpressionistUI*)(o->user_data()))->m_blue = double(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_sizeRandLightButton(Fl_Widget* o, void* v) {
    ImpressionistUI *pUI = ((ImpressionistUI*)(o->user_data()));

    if (pUI->m_sizeRand == TRUE) pUI->m_sizeRand = FALSE;
    else pUI->m_sizeRand = TRUE;
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



int ImpressionistUI::getSpacing() {
    return m_spacing;
}


void ImpressionistUI::setSpacing(int spacing) {
    m_spacing = spacing;

    if (spacing >= 1 && spacing <= 16)
        m_SpacingSlider->value(m_spacing);
}

bool ImpressionistUI::getSizeRand() {
    return m_sizeRand;
}


void ImpressionistUI::setSizeRand(bool sizeRand) {
    m_sizeRand = sizeRand;

}


void ImpressionistUI::cb_autoDrawButton(Fl_Widget* o, void* v) {
    ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
    ImpressionistDoc* pDoc = pUI->getDocument();

    //pDoc->autoDraw(((ImpressionistUI*)(o->user_data()))->m_spacing, ((ImpressionistUI*)(o->user_data()))->m_sizeRand, FALSE);
    pUI->m_paintView->autoDrawAsked = 1;
    pUI->m_paintView->redraw();
}

void ImpressionistUI::cb_filterChoice(Fl_Widget* o, void* v) {
    ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
    ImpressionistDoc* pDoc = pUI->getDocument();

    int filterChoice = (int)v;
    pDoc->curMatrix = pDoc->matrices[filterChoice];
}












// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes }, 
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas },       
        { "&Load Other(Mural) Image",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_load_other_image },  
        { "&Load Edge Image",	FL_ALT + 'h', (Fl_Callback *)ImpressionistUI::cb_load_edge_image },
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },
    { "&Function",		0, 0, 0, FL_SUBMENU },
    	{ "&Undo",        FL_ALT + 'u', (Fl_Callback *)ImpressionistUI::cb_undo },
        { "&Colors",        FL_ALT + 'o', (Fl_Callback *)ImpressionistUI::cb_color_dialog },
        { "&Exchange contents", FL_ALT + 'e', (Fl_Callback *)ImpressionistUI::cb_exchange_content},
        { "&Disolve",        FL_ALT + 'i', (Fl_Callback *)ImpressionistUI::cb_disolve },
        { 0 },
    { "&Display",		0, 0, 0, FL_SUBMENU },
        { "&Display original",	FL_ALT + 'd', (Fl_Callback *)ImpressionistUI::cbDisplayChoice, (void *)DISPLAY_MAIN },
        { "&Display the other",	FL_ALT + 'f', (Fl_Callback *)ImpressionistUI::cbDisplayChoice, (void *)DISPLAY_OTHER },
        { "&Display edge",	FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cbDisplayChoice, (void *)DISPLAY_EDGE },
        {0},


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
  {"Stars",				FL_ALT+'t', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_STARS},
  {"Coils",				FL_ALT+'o', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_COILS},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {"Filter",	        FL_ALT + 'f', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_FILTER},
  {0}
};

// Stroke direction menu definition
Fl_Menu_Item ImpressionistUI::strokeDirectionMenu[NUM_STROKE_DIRECTION_TYPE + 1] = {
    { "Slider/Right Mouse",		FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_strokeDirectionChoice, (void *)DIRECTION_SLIDER_OR_RMOUSE },
    { "Gradient",				FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_strokeDirectionChoice, (void *)DIRECTION_GRADIENT },
    { "Other Gradient",			FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_strokeDirectionChoice, (void *)DIRECTION_OTHER_GRADIENT },
    { "Brush Direction",		FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_strokeDirectionChoice, (void *)DIRECTION_BRUSH_DIRECTION },
    { 0 }
};



Fl_Menu_Item ImpressionistUI::filterChoiceMenu[2 + 1] = {
    { "Gaussian Blur",FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_filterChoice, (void *)FILTER_GAUSSIAN_BLUR },
    { "Sharpening",FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_filterChoice, (void *)FILTER_SHARPENING },
    { 0 }
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
    m_lineWidth = 1;
    m_lineAngle = 0;
    m_alpha = 1.00;
    m_spacing = 4;
    m_sizeRand = FALSE;
    m_red = 1;
    m_green = 1;
    m_blue = 1;

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");

		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);


        // Add a stroke direction choice to the dialog
        m_StrokeDirectionChoice = new Fl_Choice(115, 40, 150, 25, "&Stroke Direction");
        m_StrokeDirectionChoice->user_data((void*)(this));	 // record self to be used by static callback functions
        m_StrokeDirectionChoice->menu(strokeDirectionMenu);
        m_StrokeDirectionChoice->callback(cb_strokeDirectionChoice);
        m_StrokeDirectionChoice->deactivate();


		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
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
        m_LineWidthSlider = new Fl_Value_Slider(10, 120, 300, 20, "Line Width");
        m_LineWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
        m_LineWidthSlider->type(FL_HOR_NICE_SLIDER);
        m_LineWidthSlider->labelfont(FL_COURIER);
        m_LineWidthSlider->labelsize(12);
        m_LineWidthSlider->minimum(1);
        m_LineWidthSlider->maximum(40);
        m_LineWidthSlider->step(1);
        m_LineWidthSlider->value(m_lineWidth);
        m_LineWidthSlider->align(FL_ALIGN_RIGHT);
        m_LineWidthSlider->callback(cb_lineWidthSlides);
        m_LineWidthSlider->deactivate();

        // Add line angle slider to the dialog 
        m_LineAngleSlider = new Fl_Value_Slider(10, 160, 300, 20, "Line Angle");
        m_LineAngleSlider->user_data((void*)(this));	// record self to be used by static callback functions
        m_LineAngleSlider->type(FL_HOR_NICE_SLIDER);
        m_LineAngleSlider->labelfont(FL_COURIER);
        m_LineAngleSlider->labelsize(12);
        m_LineAngleSlider->minimum(0);
        m_LineAngleSlider->maximum(359);
        m_LineAngleSlider->step(1);
        m_LineAngleSlider->value(m_lineAngle);
        m_LineAngleSlider->align(FL_ALIGN_RIGHT);
        m_LineAngleSlider->callback(cb_lineAngleSlides);
        m_LineAngleSlider->deactivate();

        // Add alpha slider to the dialog 
        m_AlphaSlider = new Fl_Value_Slider(10, 200, 300, 20, "Alpha");
        m_AlphaSlider->user_data((void*)(this));	// record self to be used by static callback functions
        m_AlphaSlider->type(FL_HOR_NICE_SLIDER);
        m_AlphaSlider->labelfont(FL_COURIER);
        m_AlphaSlider->labelsize(12);
        m_AlphaSlider->minimum(0.0);
        m_AlphaSlider->maximum(1.0);
        m_AlphaSlider->step(0.01);
        m_AlphaSlider->value(m_alpha);
        m_AlphaSlider->align(FL_ALIGN_RIGHT);
        m_AlphaSlider->callback(cb_alphaSlides);


        // Add spacing slider to the dialog 
        m_SpacingSlider = new Fl_Value_Slider(10, 240, 120, 20, "Spacing");
        m_SpacingSlider->user_data((void*)(this));	// record self to be used by static callback functions
        m_SpacingSlider->type(FL_HOR_NICE_SLIDER);
        m_SpacingSlider->labelfont(FL_COURIER);
        m_SpacingSlider->labelsize(12);
        m_SpacingSlider->minimum(1);
        m_SpacingSlider->maximum(16);
        m_SpacingSlider->step(1);
        m_SpacingSlider->value(m_spacing);
        m_SpacingSlider->align(FL_ALIGN_RIGHT);
        m_SpacingSlider->callback(cb_spacingSlides);

        sizeRandLightButton = new Fl_Light_Button(200, 240, 100, 20, "&Random Size");
        sizeRandLightButton->user_data((void*)(this));   // record self to be used by static callback functions
        sizeRandLightButton->callback(cb_sizeRandLightButton);

        autoDrawButton = new Fl_Button(320, 240, 50, 20, "&Draw");
        autoDrawButton->user_data((void*)(this));   // record self to be used by static callback functions
        autoDrawButton->callback(cb_autoDrawButton);

        filterChoice = new Fl_Choice(50, 280, 150, 25, "filter");
        filterChoice->user_data((void*)(this));	 // record self to be used by static callback functions

        filterChoice->menu(filterChoiceMenu);
        filterChoice->callback(cb_filterChoice);


    m_brushDialog->end();	


    m_colorDialog = new Fl_Window(400, 325, "Color Dialog");

        // Add red slider to the dialog 
        m_RedSlider = new Fl_Value_Slider(10, 20, 300, 25, " Red");
        m_RedSlider->user_data((void*)(this));	// record self to be used by static callback functions
        m_RedSlider->type(FL_HOR_NICE_SLIDER);
        m_RedSlider->labelfont(FL_COURIER);
        m_RedSlider->labelsize(12);
        m_RedSlider->minimum(0);
        m_RedSlider->maximum(2);
        m_RedSlider->step(0.01);
        m_RedSlider->value(m_red);
        m_RedSlider->align(FL_ALIGN_RIGHT);
        m_RedSlider->callback(cb_redSlides);

        // Add red slider to the dialog 
        m_GreenSlider = new Fl_Value_Slider(10, 60, 300, 25, " Green");
        m_GreenSlider->user_data((void*)(this));	// record self to be used by static callback functions
        m_GreenSlider->type(FL_HOR_NICE_SLIDER);
        m_GreenSlider->labelfont(FL_COURIER);
        m_GreenSlider->labelsize(12);
        m_GreenSlider->minimum(0);
        m_GreenSlider->maximum(2);
        m_GreenSlider->step(0.01);
        m_GreenSlider->value(m_green);
        m_GreenSlider->align(FL_ALIGN_RIGHT);
        m_GreenSlider->callback(cb_greenSlides);

        // Add red slider to the dialog 
        m_BlueSlider = new Fl_Value_Slider(10, 100, 300, 25, " Blue");
        m_BlueSlider->user_data((void*)(this));	// record self to be used by static callback functions
        m_BlueSlider->type(FL_HOR_NICE_SLIDER);
        m_BlueSlider->labelfont(FL_COURIER);
        m_BlueSlider->labelsize(12);
        m_BlueSlider->minimum(0);
        m_BlueSlider->maximum(2);
        m_BlueSlider->step(0.01);
        m_BlueSlider->value(m_blue);
        m_BlueSlider->align(FL_ALIGN_RIGHT);
        m_BlueSlider->callback(cb_blueSlides);


    m_colorDialog->end();

}
