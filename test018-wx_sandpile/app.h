#pragma once

// This module introduces `App` class. The `App` class is the entry point of
// the program, and it works as a controller (as in MVC) bridging `Sandpile`
// model and `View` window.

#include <memory>

#include <wx/wx.h>

#include "sandpile.h"
#include "view.h"


struct Config {
    int width      = 29;
    int height     = 29;
    int frameDelay = 500;
};


class App : public wxApp
{
public:
    bool OnInit() override;

private:
    void bindEvents();
    void tick();
    void pause();
    void resume();
    void resetRandom();
    void resetUniform(int state);
    void increment(int x, int y);

private:
    Config                    _config;
    std::unique_ptr<Sandpile> _sandpile;
    View*                     _view  = nullptr;
    wxTimer*                  _timer = nullptr;
};
