#ifndef MEDIUMSCREENVIEW_HPP
#define MEDIUMSCREENVIEW_HPP

#include <gui_generated/mediumscreen_screen/MediumScreenViewBase.hpp>
#include <gui/mediumscreen_screen/MediumScreenPresenter.hpp>

class MediumScreenView : public MediumScreenViewBase
{
public:
    MediumScreenView();
    virtual ~MediumScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // MEDIUMSCREENVIEW_HPP
