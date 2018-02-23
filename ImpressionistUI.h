//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>

#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"

#include "ImpBrush.h"

class ImpressionistUI {
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;
								
	PaintView*			m_paintView;
	OriginalView*		m_origView;

// for brush dialog
	Fl_Window*			m_brushDialog;
    Fl_Window*			m_colorDialog;
	Fl_Choice*			m_BrushTypeChoice;
    Fl_Choice*			m_StrokeDirectionChoice;

	Fl_Slider*			m_BrushSizeSlider;
    Fl_Slider*          m_LineWidthSlider;
    Fl_Slider*          m_LineAngleSlider;
    Fl_Slider*          m_AlphaSlider;
    Fl_Slider*          m_SpacingSlider;

    Fl_Slider*          m_RedSlider;
    Fl_Slider*          m_GreenSlider;
    Fl_Slider*          m_BlueSlider;

    Fl_Light_Button*    sizeRandLightButton;
    Fl_Button*          autoDrawButton;
	Fl_Button*          m_ClearCanvasButton;
    Fl_Choice *         filterChoice;

	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*	getDocument();

	void				show();
	void				resize_windows(int w, int h);



    // getters and setters

	int					getSize();
	void				setSize(int size);

    int                 getLineWidth() {return m_lineWidth;}
    void                setLineWidth(int lineWidth) {
        
        m_lineWidth = lineWidth;

        if (lineWidth >= 1 && lineWidth <= 40) {
            m_LineWidthSlider->value(m_lineWidth);
        }
    }

    int                 getLineAngle() { return m_lineAngle; }
    void                setLineAngle(int lineAngle) {
        m_lineAngle = lineAngle;

        if (lineAngle >= 0 && lineAngle <= 359) {
            m_LineAngleSlider->value(m_lineAngle);
        }
    }

    double              getAlpha() { return m_alpha; }
    void                setAlpha(double alpha) {
        m_alpha = alpha;

        if (alpha >= 0.0 && alpha <= 1.0) {
            m_AlphaSlider->value(m_alpha);
        }
    }

    int                 getSpacing();
    void                setSpacing(int spacing);
    bool                getSizeRand();
    void                setSizeRand(bool sizeRand);

private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document

	// All attributes here
	int		m_nSize;
    int     m_lineWidth;
    int     m_lineAngle;
    double  m_alpha;
    int     m_spacing;
    bool    m_sizeRand;

public:
    double  m_red, m_green, m_blue; // modifiers from 0 to 2

private:

	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE+1];
    static Fl_Menu_Item		strokeDirectionMenu[NUM_STROKE_DIRECTION_TYPE + 1];
    static Fl_Menu_Item     filterChoiceMenu[2 + 1];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
    static void cb_load_other_image(Fl_Menu_* o, void* v);
    static void cb_load_edge_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);

    static void cb_exchange_content(Fl_Menu_* o, void* v);
    static void	cb_disolve(Fl_Menu_* o, void* v);

	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void	cb_brushChoice(Fl_Widget* o, void* v);
    static void cbDisplayChoice(Fl_Menu_* o, void* v);
    static void	cb_strokeDirectionChoice(Fl_Widget* o, void* v);
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void	cb_sizeSlides(Fl_Widget* o, void* v);

    static void	cb_color_dialog(Fl_Menu_* o, void* v);

    static void cb_redSlides(Fl_Widget* o, void* v);
    static void cb_greenSlides(Fl_Widget* o, void* v);
    static void cb_blueSlides(Fl_Widget* o, void* v);

    static void	cb_lineWidthSlides(Fl_Widget* o, void* v);
    static void	cb_lineAngleSlides(Fl_Widget* o, void* v);
    static void	cb_alphaSlides(Fl_Widget* o, void* v);
    static void	cb_spacingSlides(Fl_Widget* o, void* v);
    static void cb_sizeRandLightButton(Fl_Widget* o, void* v);
    static void cb_autoDrawButton(Fl_Widget* o, void* v);
    static void cb_filterChoice(Fl_Widget* o, void* v);

};

#endif
