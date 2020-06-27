#include "MainWindow.h"

#include <iostream>

#include <wx/wx.h>

#include "GridView.h"


MainWindow::MainWindow(wxString const& title)
{
    Create(
        nullptr,
        wxID_ANY,
        title,
        wxDefaultPosition,
        wxDefaultSize,
        wxDEFAULT_FRAME_STYLE
    );
    SetClientSize(400, 400);

    GridView::Geometry geometry;
    geometry.rows = 10;
    geometry.columns = 10;
    geometry.margin = 3;
    new GridView(this, wxID_ANY, geometry);
}
