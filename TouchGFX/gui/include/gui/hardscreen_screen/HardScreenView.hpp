#ifndef HARDSCREENVIEW_HPP
#define HARDSCREENVIEW_HPP

#include <gui_generated/hardscreen_screen/HardScreenViewBase.hpp>
#include <gui/hardscreen_screen/HardScreenPresenter.hpp>

class HardScreenView : public HardScreenViewBase
{
public:
    HardScreenView();
    virtual ~HardScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // HARDSCREENVIEW_HPP
