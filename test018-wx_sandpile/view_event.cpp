#include "view_event.h"


wxDEFINE_EVENT(VIEW_EVT_PAUSE, wxCommandEvent);
wxDEFINE_EVENT(VIEW_EVT_RESUME, wxCommandEvent);
wxDEFINE_EVENT(VIEW_EVT_RESET, ResetEvent);
wxDEFINE_EVENT(VIEW_EVT_VERTEX_CLICK, VertexClickEvent);


// ResetEvent ----------------------------------------------------------------

ResetEvent::ResetEvent(Mode mode)
    : wxCommandEvent(VIEW_EVT_RESET)
    , _mode{mode}
{
}

wxEvent*
ResetEvent::Clone() const
{
    return new ResetEvent(*this);
}

ResetEvent::Mode
ResetEvent::GetMode() const
{
    return _mode;
}

// VertexClickEvent ----------------------------------------------------------

VertexClickEvent::VertexClickEvent(int x, int y)
    : wxCommandEvent(VIEW_EVT_VERTEX_CLICK)
    , _vertex{x, y}
{
}

wxEvent*
VertexClickEvent::Clone() const
{
    return new VertexClickEvent(*this);
}

VertexClickEvent::Vertex
VertexClickEvent::GetVertex() const
{
    return _vertex;
}
