#include "GridView.h"

#include <algorithm>
#include <utility>

#include <wx/wx.h>
#include <wx/dcbuffer.h>


GridView::GridView(
    wxWindow* parent,
    wxWindowID id,
    Geometry const& geometry,
    wxPoint const& pos,
    wxSize const& size
)
    : _geometry{geometry}
    , _state{geometry.rows, geometry.columns}
{
    Create(parent, id, pos, size);

    Bind(wxEVT_PAINT, [=](wxPaintEvent& event) {
        onPaint(event);
    });

    Bind(wxEVT_LEFT_DOWN, [=](wxMouseEvent& event) {
        onClick(event);
    });
}

void GridView::onPaint(wxPaintEvent& )
{
    wxPaintDC dc{this};

    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();

    int width, height;
    dc.GetSize(&width, &height);

    auto const xSpace = (width - _geometry.margin) / _geometry.columns - _geometry.margin;
    auto const ySpace = (height - _geometry.margin) / _geometry.rows - _geometry.margin;
    auto const cellSize = std::min(xSpace, ySpace);

    dc.SetPen(*wxTRANSPARENT_PEN);

    wxBrush const* palette[] = {
        wxWHITE_BRUSH,
        wxCYAN_BRUSH,
        wxYELLOW_BRUSH,
        wxRED_BRUSH,
    };
    constexpr int paletteSize = sizeof palette / sizeof *palette;

    for (int row = 0; row < _geometry.rows; row++) {
        for (int col = 0; col < _geometry.columns; col++) {
            wxPoint const pos = {
                col * (cellSize + _geometry.margin) + _geometry.margin,
                row * (cellSize + _geometry.margin) + _geometry.margin
            };

            dc.SetBrush(*palette[_state(row, col) % paletteSize]);

            dc.DrawRectangle(pos, {cellSize, cellSize});
        }
    }
}

void GridView::onClick(wxMouseEvent& event)
{
    wxClientDC dc{this};

    // FIXME: Model operation in view.

    int width, height;
    dc.GetSize(&width, &height);

    auto const xSpace = (width - _geometry.margin) / _geometry.columns - _geometry.margin;
    auto const ySpace = (height - _geometry.margin) / _geometry.rows - _geometry.margin;
    auto const cellSize = std::min(xSpace, ySpace);
    auto const cellPeriod = cellSize + _geometry.margin;

    auto const clickPos = event.GetLogicalPosition(dc);

    auto const column = (clickPos.x - _geometry.margin) / cellPeriod;
    auto const columnX = (clickPos.x - _geometry.margin) % cellPeriod;
    auto const row = (clickPos.y - _geometry.margin) / cellPeriod;
    auto const rowY = (clickPos.y - _geometry.margin) % cellPeriod;

    if (row < 0 || row >= _geometry.rows) {
        return;
    }
    if (column < 0 || column >= _geometry.columns) {
        return;
    }
    if (columnX < 0 || columnX >= cellSize) {
        return;
    }
    if (rowY < 0 || rowY >= cellSize) {
        return;
    }

    _state(row, column)++;

    Refresh();
}
