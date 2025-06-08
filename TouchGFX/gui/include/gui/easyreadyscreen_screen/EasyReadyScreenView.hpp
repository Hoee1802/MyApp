#ifndef EASYREADYSCREENVIEW_HPP
#define EASYREADYSCREENVIEW_HPP

#include <gui_generated/easyreadyscreen_screen/EasyReadyScreenViewBase.hpp>
#include <gui/easyreadyscreen_screen/EasyReadyScreenPresenter.hpp>

class EasyReadyScreenView : public EasyReadyScreenViewBase
{
public:
    EasyReadyScreenView();
    virtual ~EasyReadyScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // EASYREADYSCREENVIEW_HPP
