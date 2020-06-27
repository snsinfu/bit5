#pragma once

#include <wx/wx.h>

#include "Counter.h"
#include "View.h"


class App : public wxApp
{
public:
    bool OnInit() override;

private:
    Counter  _counter;
    View*    _view  = nullptr;
    wxTimer* _timer = nullptr;
};
