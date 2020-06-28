#pragma once

#include <wx/wx.h>


class ResetEvent;
class VertexClickEvent;

wxDECLARE_EVENT(VIEW_EVT_PAUSE, wxCommandEvent);
wxDECLARE_EVENT(VIEW_EVT_RESUME, wxCommandEvent);
wxDECLARE_EVENT(VIEW_EVT_RESET, ResetEvent);
wxDECLARE_EVENT(VIEW_EVT_VERTEX_CLICK, VertexClickEvent);


class ResetEvent : public wxCommandEvent
{
public:
    enum class Mode {
        ZERO,
        ONE,
        TWO,
        THREE,
        RANDOM,
    };

    explicit ResetEvent(Mode mode);

    wxEvent* Clone() const override;
    Mode     GetMode() const;

private:
    Mode _mode;
};


class VertexClickEvent : public wxCommandEvent
{
public:
    struct Vertex {
        int x = 0;
        int y = 0;
    };

    VertexClickEvent(int x, int y);

    wxEvent* Clone() const override;
    Vertex   GetVertex() const;

private:
    Vertex _vertex;
};
