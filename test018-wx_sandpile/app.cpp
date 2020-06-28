#include "app.h"

#include <memory>
#include <random>

#include "sandpile.h"
#include "view.h"
#include "view_event.h"


wxIMPLEMENT_APP(App);


bool
App::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }

    _sandpile = std::make_unique<Sandpile>(_config.width, _config.height);
    _view = new View(this);
    _timer = new wxTimer(this);

    randomize(*_sandpile);
    _view->setSandpile(_sandpile.get());

    bindEvents();

    return true;
}

void
App::bindEvents()
{
    Bind(wxEVT_TIMER, [=](auto&) {
        tick();
    });

    Bind(VIEW_EVT_PAUSE, [=](auto&) {
        pause();
    });

    Bind(VIEW_EVT_RESUME, [=](auto&) {
        resume();
    });

    Bind(VIEW_EVT_RESET, [=](ResetEvent& event) {
        switch (event.GetMode()) {
        case ResetEvent::Mode::RANDOM:
            resetRandom();
            break;

        case ResetEvent::Mode::ZERO:
        case ResetEvent::Mode::ONE:
        case ResetEvent::Mode::TWO:
        case ResetEvent::Mode::THREE:
            resetUniform(int(event.GetMode()));
            break;
        }
    });

    Bind(VIEW_EVT_VERTEX_CLICK, [=](VertexClickEvent& event) {
        auto [x, y] = event.GetVertex();
        increment(x, y);
    });
}

void
App::tick()
{
    _sandpile->tick();
    _view->setSandpile(_sandpile.get());
}

void
App::pause()
{
    _timer->Stop();
}

void
App::resume()
{
    _timer->Start(_config.frameDelay);
    tick();
}

void
App::resetRandom()
{
    randomize(*_sandpile);
    _view->setSandpile(_sandpile.get());
}

void
App::resetUniform(int state)
{
    initialize(*_sandpile, state);
    _view->setSandpile(_sandpile.get());
}

void
App::increment(int x, int y)
{
    (*_sandpile)(x, y)++;
    _view->setSandpile(_sandpile.get());
}
