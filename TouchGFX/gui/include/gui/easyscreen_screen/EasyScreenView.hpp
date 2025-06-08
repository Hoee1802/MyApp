#ifndef EASYSCREENVIEW_HPP
#define EASYSCREENVIEW_HPP

#include <gui_generated/easyscreen_screen/EasyScreenViewBase.hpp>
#include <gui/easyscreen_screen/EasyScreenPresenter.hpp>

class EasyScreenView : public EasyScreenViewBase
{
public:
    EasyScreenView();
    virtual ~EasyScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // EASYSCREENVIEW_HPP
