#ifndef HARDREADYSCREENPRESENTER_HPP
#define HARDREADYSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class HardReadyScreenView;

class HardReadyScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    HardReadyScreenPresenter(HardReadyScreenView& v);

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

    virtual ~HardReadyScreenPresenter() {}

private:
    HardReadyScreenPresenter();

    HardReadyScreenView& view;
};

#endif // HARDREADYSCREENPRESENTER_HPP
