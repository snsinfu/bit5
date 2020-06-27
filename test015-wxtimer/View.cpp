#include "View.h"

#include <wx/wx.h>


wxDEFINE_EVENT(VIEW_EVT_RESET, wxCommandEvent);
wxDEFINE_EVENT(VIEW_EVT_PICK, PickEvent);


// View ----------------------------------------------------------------------

View::View(wxEvtHandler* parent)
{
    Create(
        nullptr,
        wxID_ANY,
        "counter",
        wxDefaultPosition,
        wxDefaultSize,
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE)
    );
    SetClientSize(500, 400);

    _canvas = new wxControl(
        this, wxID_ANY, wxPoint{10, 10}, wxSize{380, 380}
    );

    _resetButton = new wxButton(
        this, wxID_ANY, "Reset", wxPoint{400, 10}, wxSize{90, 25}
    );

    Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
        wxPostEvent(parent, wxCommandEvent{VIEW_EVT_RESET});
    });

    _canvas->Bind(wxEVT_PAINT, [=](wxPaintEvent&) {
        wxPaintDC dc{_canvas};
        DrawCanvas(dc);
    });

    _canvas->Bind(wxEVT_LEFT_DOWN, [=](wxMouseEvent& event) {
        wxClientDC dc{_canvas};
        auto const pos = event.GetLogicalPosition(dc);

        auto const col = (pos.x - 10) / 10;
        auto const row = (pos.y - 40) / 10;
        if (col < 0 || col >= 36) {
            return;
        }
        if (row < 0 || row >= 33) {
            return;
        }
        auto const choice = row * 36 + col;

        PickEvent pick;
        pick.SetChoice(choice);
        wxPostEvent(parent, pick);
    });
}

void
View::Start()
{
    Show();
}

void
View::SetCount(int count)
{
    _count = count;
    Refresh();
}

void
View::DrawCanvas(wxDC& dc) const
{
    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();

    dc.SetTextForeground(*wxWHITE);
    wxString text;
    text.Printf("%d", _count);
    dc.DrawText(text, 10, 10);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(*wxRED_BRUSH);

    for (int i = 0; i < _count; i++) {
        int x = (i % 36) * 10 + 10;
        int y = (i / 36) * 10 + 40;
        dc.DrawRectangle(wxPoint{x, y}, wxSize{8, 8});
    }
}


// PickEvent -----------------------------------------------------------------

PickEvent::PickEvent()
    : wxCommandEvent{VIEW_EVT_PICK}
{
}

wxEvent*
PickEvent::Clone() const
{
    return new PickEvent{*this};
}

void
PickEvent::SetChoice(int choice)
{
    _choice = choice;
}

int
PickEvent::GetChoice() const
{
    return _choice;
}
