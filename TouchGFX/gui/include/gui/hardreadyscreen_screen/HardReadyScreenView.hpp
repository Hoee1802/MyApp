#ifndef HARDREADYSCREENVIEW_HPP
#define HARDREADYSCREENVIEW_HPP

#include <gui_generated/hardreadyscreen_screen/HardReadyScreenViewBase.hpp>
#include <gui/hardreadyscreen_screen/HardReadyScreenPresenter.hpp>

class HardReadyScreenView : public HardReadyScreenViewBase
{
public:
    HardReadyScreenView();
    virtual ~HardReadyScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // HARDREADYSCREENVIEW_HPP
