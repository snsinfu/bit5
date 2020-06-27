#pragma once

#include <wx/wx.h>


class PickEvent;

wxDECLARE_EVENT(VIEW_EVT_RESET, wxCommandEvent);
wxDECLARE_EVENT(VIEW_EVT_PICK, PickEvent);


class View : private wxFrame
{
public:
    explicit View(wxEvtHandler* parent);
    void Start();
    void SetCount(int count);

private:
    void DrawCanvas(wxDC& dc) const;

private:
    wxControl* _canvas      = nullptr;
    wxButton*  _resetButton = nullptr;
    int        _count       = 0;
};


class PickEvent : public wxCommandEvent
{
public:
    PickEvent();
    wxEvent* Clone() const override;
    void SetChoice(int choice);
    int  GetChoice() const;

private:
    int _choice = -1;
};
