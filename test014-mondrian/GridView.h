#pragma once

#include <Eigen/Dense>
#include <wx/wx.h>

#include "Matrix.h"


class GridView : public wxControl
{
public:
    struct Geometry {
        int rows    = 1;
        int columns = 1;
        int margin  = 0;
    };

    explicit GridView(
        wxWindow* parent,
        wxWindowID id,
        Geometry const& geometry,
        wxPoint const& pos = wxDefaultPosition,
        wxSize const& size = wxDefaultSize
    );

private:
    void onPaint(wxPaintEvent& event);
    void onClick(wxMouseEvent& event);

private:
    Geometry    _geometry;
    Matrix<int> _state;
};
