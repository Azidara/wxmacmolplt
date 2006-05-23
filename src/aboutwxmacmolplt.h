/////////////////////////////////////////////////////////////////////////////
// Name:        aboutwxmacmolplt.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Tue 23 May 13:01:58 2006
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), Tue 23 May 13:01:58 2006

#ifndef _ABOUTWXMACMOLPLT_H_
#define _ABOUTWXMACMOLPLT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "aboutwxmacmolplt.h"
#endif

/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DIALOG 10053
#define SYMBOL_ABOUTWXMACMOLPLT_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ABOUTWXMACMOLPLT_TITLE _("About wxMacMolPlt")
#define SYMBOL_ABOUTWXMACMOLPLT_IDNAME ID_DIALOG
#define SYMBOL_ABOUTWXMACMOLPLT_SIZE wxSize(400, 300)
#define SYMBOL_ABOUTWXMACMOLPLT_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * AboutwxMacMolPlt class declaration
 */

class AboutwxMacMolPlt: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( AboutwxMacMolPlt )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    AboutwxMacMolPlt( );
    AboutwxMacMolPlt( wxWindow* parent, wxWindowID id = SYMBOL_ABOUTWXMACMOLPLT_IDNAME, const wxString& caption = SYMBOL_ABOUTWXMACMOLPLT_TITLE, const wxPoint& pos = SYMBOL_ABOUTWXMACMOLPLT_POSITION, const wxSize& size = SYMBOL_ABOUTWXMACMOLPLT_SIZE, long style = SYMBOL_ABOUTWXMACMOLPLT_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ABOUTWXMACMOLPLT_IDNAME, const wxString& caption = SYMBOL_ABOUTWXMACMOLPLT_TITLE, const wxPoint& pos = SYMBOL_ABOUTWXMACMOLPLT_POSITION, const wxSize& size = SYMBOL_ABOUTWXMACMOLPLT_SIZE, long style = SYMBOL_ABOUTWXMACMOLPLT_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin AboutwxMacMolPlt event handler declarations

////@end AboutwxMacMolPlt event handler declarations

////@begin AboutwxMacMolPlt member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end AboutwxMacMolPlt member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin AboutwxMacMolPlt member variables
////@end AboutwxMacMolPlt member variables
};

#endif
    // _ABOUTWXMACMOLPLT_H_