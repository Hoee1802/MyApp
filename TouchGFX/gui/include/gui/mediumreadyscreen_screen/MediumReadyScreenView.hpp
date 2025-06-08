#ifndef MEDIUMREADYSCREENVIEW_HPP
#define MEDIUMREADYSCREENVIEW_HPP

#include <gui_generated/mediumreadyscreen_screen/MediumReadyScreenViewBase.hpp>
#include <gui/mediumreadyscreen_screen/MediumReadyScreenPresenter.hpp>

class MediumReadyScreenView : public MediumReadyScreenViewBase
{
public:
    MediumReadyScreenView();
    virtual ~MediumReadyScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // MEDIUMREADYSCREENVIEW_HPP
