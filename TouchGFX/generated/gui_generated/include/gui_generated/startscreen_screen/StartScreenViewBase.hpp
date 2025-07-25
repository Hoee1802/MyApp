/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef STARTSCREENVIEWBASE_HPP
#define STARTSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/startscreen_screen/StartScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/TextArea.hpp>

class StartScreenViewBase : public touchgfx::View<StartScreenPresenter>
{
public:
    StartScreenViewBase();
    virtual ~StartScreenViewBase();
    virtual void setupScreen();

    /*
     * Custom Actions
     */
    virtual void action1()
    {
        // Override and implement this function in Screen1
    }
    

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Box background;
    touchgfx::ButtonWithLabel buttonHard;
    touchgfx::ButtonWithLabel buttonEasy;
    touchgfx::Circle circle5_1_4;
    touchgfx::PainterRGB565 circle5_1_4Painter;
    touchgfx::Circle circle5_1_3;
    touchgfx::PainterRGB565 circle5_1_3Painter;
    touchgfx::Circle circle5_4;
    touchgfx::PainterRGB565 circle5_4Painter;
    touchgfx::Circle circle5_2_1;
    touchgfx::PainterRGB565 circle5_2_1Painter;
    touchgfx::Circle circle5_2;
    touchgfx::PainterRGB565 circle5_2Painter;
    touchgfx::Circle circle5_1_1;
    touchgfx::PainterRGB565 circle5_1_1Painter;
    touchgfx::Circle circle5_1;
    touchgfx::PainterRGB565 circle5_1Painter;
    touchgfx::TextArea titleText;
    touchgfx::Circle circle1;
    touchgfx::PainterRGB565 circle1Painter;
    touchgfx::Circle circle2;
    touchgfx::PainterRGB565 circle2Painter;
    touchgfx::Circle circle3;
    touchgfx::PainterRGB565 circle3Painter;
    touchgfx::Circle circle4;
    touchgfx::PainterRGB565 circle4Painter;
    touchgfx::ButtonWithLabel buttonMedium;

private:

    /*
     * Canvas Buffer Size
     */
    static const uint32_t CANVAS_BUFFER_SIZE = 4800;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];

    /*
     * Callback Declarations
     */
    touchgfx::Callback<StartScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // STARTSCREENVIEWBASE_HPP
