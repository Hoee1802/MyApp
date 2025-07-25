#ifndef EASYREADYSCREENPRESENTER_HPP
#define EASYREADYSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class EasyReadyScreenView;

class EasyReadyScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    EasyReadyScreenPresenter(EasyReadyScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~EasyReadyScreenPresenter() {}

private:
    EasyReadyScreenPresenter();

    EasyReadyScreenView& view;
};

#endif // EASYREADYSCREENPRESENTER_HPP
