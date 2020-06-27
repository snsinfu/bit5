#include "App.h"

#include <iostream>

#include <wx/wx.h>


wxIMPLEMENT_APP(App);


bool App::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }

    _view = new View(this);
    _view->Start();
    _counter.Reset();

    _timer = new wxTimer(this);
    _timer->Start(500);

    Bind(wxEVT_TIMER, [=](wxTimerEvent&) {
        _counter.Tick();
        _view->SetCount(_counter.Get());
    });

    Bind(VIEW_EVT_RESET, [=](wxCommandEvent&) {
        _counter.Reset();
        _view->SetCount(_counter.Get());
    });

    Bind(VIEW_EVT_PICK, [=](PickEvent& event) {
        _counter.Reset(event.GetChoice());
        _view->SetCount(_counter.Get());
    });

    return true;
}
