#include "view.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <utility>

#include "misc/matrix.h"
#include "sandpile.h"
#include "view_event.h"


static std::array<wxBrush const*, 5> const stateBrushes = {
    wxGREY_BRUSH,
    wxBLUE_BRUSH,
    wxCYAN_BRUSH,
    wxYELLOW_BRUSH,
    wxRED_BRUSH
};

static std::array<ResetEvent::Mode, 5> const resetChoices = {
    ResetEvent::Mode::ZERO,
    ResetEvent::Mode::ONE,
    ResetEvent::Mode::TWO,
    ResetEvent::Mode::THREE,
    ResetEvent::Mode::RANDOM
};

static std::array<wxString, 5> const resetChoicesLabels = {
    "All zero",
    "All one",
    "All two",
    "All three",
    "Random"
};

static constexpr int resetChoiceDefault = 4; // RANDOM


View::View(wxEvtHandler* owner)
    : _owner{owner}
{
    createGUI();
    bindEvents();
    wxFrame::Show();
}

void
View::createGUI()
{
    wxFrame::Create(
        nullptr,
        wxID_ANY,
        "sandpile",
        wxDefaultPosition,
        wxDefaultSize,
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)
    );
    wxFrame::SetClientSize(500, 400);

    _canvas = new wxControl(
        this, wxID_ANY, wxPoint(10, 10), wxSize(380, 380)
    );

    _resetChoice = new wxChoice(
        this, wxID_ANY, wxPoint(400, 10), wxSize(90, 25)
    );
    for (auto const& label : resetChoicesLabels) {
        _resetChoice->Append(label);
    }
    _resetChoice->SetSelection(resetChoiceDefault);

    _resetButton = new wxButton(
        this, wxID_ANY, "Reset", wxPoint(400, 40), wxSize(90, 25)
    );

    _runButton = new wxToggleButton(
        this, wxID_ANY, "Run", wxPoint(400, 365), wxSize(90, 25)
    );
}

void
View::bindEvents()
{
    _canvas->Bind(wxEVT_PAINT, [=](auto& event) {
        onCanvasPaint(event);
    });

    _canvas->Bind(wxEVT_LEFT_DOWN, [=](auto& event) {
        onCanvasClick(event);
    });

    _resetButton->Bind(wxEVT_BUTTON, [=](auto& event) {
        onResetButtonClick(event);
    });

    _runButton->Bind(wxEVT_TOGGLEBUTTON, [=](auto& event) {
        onRunButtonClick(event);
    });
}

void
View::setSandpile(Sandpile const* sandpile)
{
    _sandpile = sandpile;

    if (!_sandpile) {
        return;
    }

    auto const xcount = _sandpile->width();
    auto const ycount = _sandpile->height();
    int w, h;
    _canvas->GetClientSize(&w, &h);

    auto const pad = 10;
    auto const xsize = (w - 2 * pad) / xcount;
    auto const ysize = (h - 2 * pad) / ycount;
    auto const vertexSize = std::min(xsize, ysize) - 1;

    auto const xmargin = (w - 2 * pad - vertexSize * xcount) / xcount;
    auto const ymargin = (h - 2 * pad - vertexSize * ycount) / ycount;
    auto const margin = std::min(xmargin, ymargin);

    auto const span = margin + vertexSize;
    auto const xpad = (w - span * xcount) / 2;
    auto const ypad = (h - span * ycount) / 2;

    _geometry.xcount = xcount;
    _geometry.ycount = ycount;
    _geometry.xpad = xpad;
    _geometry.ypad = ypad;
    _geometry.margin = margin;
    _geometry.vertexSize = vertexSize;

    _canvas->Refresh();
}

void
View::onCanvasPaint(wxPaintEvent&)
{
    wxPaintDC dc(_canvas);
    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();

    if (!_sandpile) {
        return;
    }
    auto& sandpile = *_sandpile;

    dc.SetPen(*wxTRANSPARENT_PEN);

    auto const cellSpan = _geometry.vertexSize + _geometry.margin;

    for (int y = 0; y < sandpile.height(); y++) {
        for (int x = 0; x < sandpile.width(); x++) {
            int const state = sandpile(x, y);

            auto const brushIndex = std::min(std::size_t(state), stateBrushes.size() - 1);
            dc.SetBrush(*stateBrushes.at(brushIndex));

            wxPoint const pos = {
                _geometry.xpad + cellSpan * x,
                _geometry.ypad + cellSpan * y
            };
            wxSize const size = {
                _geometry.vertexSize,
                _geometry.vertexSize
            };
            dc.DrawRectangle(pos, size);
        }
    }
}

void
View::onCanvasClick(wxMouseEvent& event)
{
    wxClientDC dc(_canvas);
    auto const pos = event.GetLogicalPosition(dc);
    auto const cellSpan = _geometry.vertexSize + _geometry.margin;
    auto const x = (pos.x - _geometry.xpad) / cellSpan;
    auto const y = (pos.y - _geometry.ypad) / cellSpan;

    if (x < 0 || x >= _geometry.xcount) {
        return;
    }
    if (y < 0 || y >= _geometry.ycount) {
        return;
    }

    wxPostEvent(_owner, VertexClickEvent(x, y));
}

void
View::onResetButtonClick(wxCommandEvent&)
{
    auto const mode = resetChoices.at(
        static_cast<std::size_t>(_resetChoice->GetCurrentSelection())
    );
    wxPostEvent(_owner, ResetEvent(mode));
}

void
View::onRunButtonClick(wxCommandEvent&)
{
    if (_runButton->GetValue()) {
        wxPostEvent(_owner, wxCommandEvent(VIEW_EVT_RESUME));
    } else {
        wxPostEvent(_owner, wxCommandEvent(VIEW_EVT_PAUSE));
    }
}
