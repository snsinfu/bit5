#include <wx/wx.h>

#include "App.h"
#include "MainWindow.h"


wxIMPLEMENT_APP(App);


bool App::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }

    auto mainWindow = new MainWindow("Mondrian");
    mainWindow->Show(true);

    return true;
}
