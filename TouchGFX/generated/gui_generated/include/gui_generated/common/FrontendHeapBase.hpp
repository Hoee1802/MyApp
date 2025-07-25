/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef FRONTENDHEAPBASE_HPP
#define FRONTENDHEAPBASE_HPP

#include <common/Meta.hpp>
#include <common/Partition.hpp>
#include <mvp/MVPHeap.hpp>

#include <touchgfx/transitions/NoTransition.hpp>
#include <touchgfx/transitions/WipeTransition.hpp>

#include <gui/common/FrontendApplication.hpp>
#include <gui/model/Model.hpp>

#include <gui/startscreen_screen/StartScreenView.hpp>
#include <gui/startscreen_screen/StartScreenPresenter.hpp>
#include <gui/hardscreen_screen/HardScreenView.hpp>
#include <gui/hardscreen_screen/HardScreenPresenter.hpp>
#include <gui/hardreadyscreen_screen/HardReadyScreenView.hpp>
#include <gui/hardreadyscreen_screen/HardReadyScreenPresenter.hpp>
#include <gui/mediumscreen_screen/MediumScreenView.hpp>
#include <gui/mediumscreen_screen/MediumScreenPresenter.hpp>
#include <gui/mediumreadyscreen_screen/MediumReadyScreenView.hpp>
#include <gui/mediumreadyscreen_screen/MediumReadyScreenPresenter.hpp>
#include <gui/easyscreen_screen/EasyScreenView.hpp>
#include <gui/easyscreen_screen/EasyScreenPresenter.hpp>
#include <gui/easyreadyscreen_screen/EasyReadyScreenView.hpp>
#include <gui/easyreadyscreen_screen/EasyReadyScreenPresenter.hpp>
#include <gui/endscreen_screen/EndScreenView.hpp>
#include <gui/endscreen_screen/EndScreenPresenter.hpp>


/**
 * This class provides the memory that shall be used for memory allocations
 * in the frontend. A single instance of the FrontendHeap is allocated once (in heap
 * memory), and all other frontend objects such as views, presenters and data model are
 * allocated within the scope of this FrontendHeap. As such, the RAM usage of the entire
 * user interface is sizeof(FrontendHeap).
 *
 * @note The FrontendHeap reserves memory for the most memory-consuming presenter and
 * view only. The largest of these classes are determined at compile-time using template
 * magic. As such, it is important to add all presenters, views and transitions to the
 * type lists in this class.
 *
 */
class FrontendHeapBase : public touchgfx::MVPHeap
{
public:
    /**
     * A list of all view types. Must end with meta::Nil.
     * @note All view types used in the application MUST be added to this list!
     */
    typedef touchgfx::meta::TypeList< StartScreenView,
            touchgfx::meta::TypeList< HardScreenView,
            touchgfx::meta::TypeList< HardReadyScreenView,
            touchgfx::meta::TypeList< MediumScreenView,
            touchgfx::meta::TypeList< MediumReadyScreenView,
            touchgfx::meta::TypeList< EasyScreenView,
            touchgfx::meta::TypeList< EasyReadyScreenView,
            touchgfx::meta::TypeList< EndScreenView,
            touchgfx::meta::Nil > > > > > > >
            > GeneratedViewTypes;

    /**
     * Determine (compile time) the View type of largest size.
     */
    typedef touchgfx::meta::select_type_maxsize< GeneratedViewTypes >::type MaxGeneratedViewType;

    /**
     * A list of all presenter types. Must end with meta::Nil.
     * @note All presenter types used in the application MUST be added to this list!
     */
    typedef touchgfx::meta::TypeList< StartScreenPresenter,
            touchgfx::meta::TypeList< HardScreenPresenter,
            touchgfx::meta::TypeList< HardReadyScreenPresenter,
            touchgfx::meta::TypeList< MediumScreenPresenter,
            touchgfx::meta::TypeList< MediumReadyScreenPresenter,
            touchgfx::meta::TypeList< EasyScreenPresenter,
            touchgfx::meta::TypeList< EasyReadyScreenPresenter,
            touchgfx::meta::TypeList< EndScreenPresenter,
            touchgfx::meta::Nil > > > > > > >
            > GeneratedPresenterTypes;

    /**
     * Determine (compile time) the Presenter type of largest size.
     */
    typedef touchgfx::meta::select_type_maxsize< GeneratedPresenterTypes >::type MaxGeneratedPresenterType;

    /**
     * A list of all transition types. Must end with meta::Nil.
     * @note All transition types used in the application MUST be added to this list!
     */
    typedef touchgfx::meta::TypeList< touchgfx::NoTransition,
            touchgfx::meta::TypeList< WipeTransition<EAST>,
            touchgfx::meta::Nil >
            > GeneratedTransitionTypes;

    /**
     * Determine (compile time) the Transition type of largest size.
     */
    typedef touchgfx::meta::select_type_maxsize< GeneratedTransitionTypes >::type MaxGeneratedTransitionType;

    virtual void gotoStartScreen(FrontendApplication& app)
    {
        app.gotoStartScreenScreenNoTransition();
    }
protected:
    FrontendHeapBase(touchgfx::AbstractPartition& presenters, touchgfx::AbstractPartition& views, touchgfx::AbstractPartition& transitions, FrontendApplication& app)
        : MVPHeap(presenters, views, transitions, app)
    {

    }

};

#endif // FRONTENDHEAPBASE_HPP
