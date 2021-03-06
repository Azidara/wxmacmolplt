/////////////////////////////////////////////////////////////////////////////
// Name:        exportoptionsdialog.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Thu 11 May 2006 02:46:50 PM CDT
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), Thu 11 May 2006 02:46:50 PM CDT

#ifndef _EXPORTOPTIONSDIALOG_H_
#define _EXPORTOPTIONSDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "exportoptionsdialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/spinctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSpinCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_EXPORTOPTIONSDIALOG 10044
#define ID_RES_PANEL 10045
#define ID_RES_CHOICE 10047
#define ID_RESWIDTHSPIN 10051
#define ID_RESHEIGHTSPIN 10049
#define ID_CHECKBOX3 10260
#define ID_ANIMATIONRADIO 10000
#define ID_ENERGYPLOTCHECK 10001
#define SYMBOL_EXPORTOPTIONSDIALOG_STYLE wxDEFAULT_DIALOG_STYLE|wxCAPTION|wxSYSTEM_MENU|wxSTAY_ON_TOP
#define SYMBOL_EXPORTOPTIONSDIALOG_TITLE _("Export Options")
#define SYMBOL_EXPORTOPTIONSDIALOG_IDNAME ID_EXPORTOPTIONSDIALOG
#define SYMBOL_EXPORTOPTIONSDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_EXPORTOPTIONSDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * ExportOptionsDialog class declaration
 */

class ExportOptionsDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ExportOptionsDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ExportOptionsDialog( );
    ExportOptionsDialog( wxWindow* parent, wxWindowID id = SYMBOL_EXPORTOPTIONSDIALOG_IDNAME, const wxString& caption = SYMBOL_EXPORTOPTIONSDIALOG_TITLE, const wxPoint& pos = SYMBOL_EXPORTOPTIONSDIALOG_POSITION, const wxSize& size = SYMBOL_EXPORTOPTIONSDIALOG_SIZE, long style = SYMBOL_EXPORTOPTIONSDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_EXPORTOPTIONSDIALOG_IDNAME, const wxString& caption = SYMBOL_EXPORTOPTIONSDIALOG_TITLE, const wxPoint& pos = SYMBOL_EXPORTOPTIONSDIALOG_POSITION, const wxSize& size = SYMBOL_EXPORTOPTIONSDIALOG_SIZE, long style = SYMBOL_EXPORTOPTIONSDIALOG_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin ExportOptionsDialog event handler declarations

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_RES_CHOICE
	void OnResChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_RESWIDTHSPIN
	void OnReswidthspinUpdated( wxSpinEvent& event );

	/// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_RESHEIGHTSPIN
	void OnResheightspinUpdated( wxSpinEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX3
	void OnCheckbox3Click( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_ANIMATIONRADIO
	void OnAnimationradioSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
	void OnOkClick( wxCommandEvent& event );

////@end ExportOptionsDialog event handler declarations

	/// returns the desired width of the image
    int getWidth() const { return width; }
	/// returns the desired height of the image
    int getHeight() const { return height; }
	/// returns true if the target image should have a transparent background
    bool getTransparency() const { return transparency; }
	/// returns the ratio between the desired image and the screen size
	int getImageRatio() const;
	/// Set the type of movie to export
	void SetMovieChoice(int v);
	/// Call to enable/disable selecting a frame movie
	void EnableFrameMovie(bool v);
	/// Call to enable/disable selecting a normal mode movie
	void EnableModeMovie(bool b);
	/// Return the user selected movie type
	int GetMovieChoice(void) const;
	/// Return the user selection for adding the energy plot to the movie
	bool AddEnergyPlot(void) const;
	
    void setFileType(int ft);

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );

    /// Should we show tooltips?
    static bool ShowToolTips();

private:
    long width;
    long height;
    long screenWidth;
    long screenHeight;
    double H2Wratio;
    int filetype;
    bool transparency;

    wxChoice        *fileTypeChoice;
    wxChoice        *resChoice;
    wxStaticText    *txt0;
    wxStaticText    *txt1;
    wxStaticText    *txt2;
    wxStaticText    *txt3;
    wxSpinCtrl      *resWidthSpin;
    wxSpinCtrl      *resHeightSpin;
    wxPanel         *panelRes;
    wxCheckBox      *resTBGCheck;
	wxRadioBox		*mMovieType;
	wxCheckBox		*mEPlotCheck;
};

#endif
    // _EXPORTOPTIONSDIALOG_H_
