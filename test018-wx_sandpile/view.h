#pragma once

#include <wx/tglbtn.h>
#include <wx/wx.h>

#include "misc/matrix.h"
#include "sandpile.h"
#include "view_event.h"


class View : protected wxFrame
{
    struct Geometry {
        int xcount     = 0;
        int ycount     = 0;
        int xpad       = 0;
        int ypad       = 0;
        int margin     = 0;
        int vertexSize = 0;
    };

public:
    explicit View(wxEvtHandler* controller);

    void setSandpile(Sandpile const* sandpile);

private:
    void createGUI();
    void bindEvents();

    void onCanvasPaint(wxPaintEvent&);
    void onCanvasClick(wxMouseEvent&);
    void onResetButtonClick(wxCommandEvent&);
    void onRunButtonClick(wxCommandEvent&);

private:
    wxEvtHandler*   _owner       = nullptr;
    wxControl*      _canvas      = nullptr;
    wxChoice*       _resetChoice = nullptr;
    wxButton*       _resetButton = nullptr;
    wxToggleButton* _runButton   = nullptr;
    Sandpile const* _sandpile    = nullptr;
    Geometry        _geometry;
};
