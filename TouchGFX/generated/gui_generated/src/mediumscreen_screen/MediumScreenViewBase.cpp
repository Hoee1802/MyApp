/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/mediumscreen_screen/MediumScreenViewBase.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

MediumScreenViewBase::MediumScreenViewBase()
{
    touchgfx::CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);
    
    __background.setPosition(0, 0, 320, 240);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    box3.setPosition(0, 0, 320, 240);
    box3.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    add(box3);

    box1.setPosition(2, 2, 316, 236);
    box1.setColor(touchgfx::Color::getColorFromRGB(12, 56, 55));
    add(box1);

    box2.setPosition(160, 0, 3, 240);
    box2.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    box2.setAlpha(220);
    add(box2);

    paddle1.setPosition(7, 101, 5, 40);
    paddle1.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    add(paddle1);

    paddle2.setPosition(308, 101, 5, 40);
    paddle2.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    add(paddle2);

    score2.setXY(184, 22);
    score2.setColor(touchgfx::Color::getColorFromRGB(89, 86, 86));
    score2.setLinespacing(0);
    score2.setRotation(touchgfx::TEXT_ROTATE_270);
    Unicode::snprintf(score2Buffer, SCORE2_SIZE, "%s", touchgfx::TypedText(T_SCORE2BUFFER2).getText());
    score2.setWildcard(score2Buffer);
    score2.resizeToCurrentText();
    score2.setTypedText(touchgfx::TypedText(T___SINGLEUSE_KK9M));
    add(score2);

    score1.setXY(117, 195);
    score1.setColor(touchgfx::Color::getColorFromRGB(89, 86, 86));
    score1.setLinespacing(0);
    score1.setRotation(touchgfx::TEXT_ROTATE_90);
    Unicode::snprintf(score1Buffer, SCORE1_SIZE, "%s", touchgfx::TypedText(T_SCORE1BUFFER2).getText());
    score1.setWildcard(score1Buffer);
    score1.resizeToCurrentText();
    score1.setTypedText(touchgfx::TypedText(T___SINGLEUSE_ATHF));
    add(score1);

    player1.setPosition(140, 180, 19, 52);
    player1.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    player1.setLinespacing(0);
    player1.setRotation(touchgfx::TEXT_ROTATE_90);
    player1.setTypedText(touchgfx::TypedText(T___SINGLEUSE_V9P9));
    player1.setAlpha(190);
    add(player1);

    player2.setPosition(165, 7, 19, 52);
    player2.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    player2.setLinespacing(0);
    player2.setRotation(touchgfx::TEXT_ROTATE_270);
    player2.setTypedText(touchgfx::TypedText(T___SINGLEUSE_UJJ8));
    player2.setAlpha(190);
    add(player2);

    goal2.setPosition(318, 60, 2, 120);
    goal2.setColor(touchgfx::Color::getColorFromRGB(0, 11, 110));
    goal2.setBorderColor(touchgfx::Color::getColorFromRGB(173, 147, 52));
    goal2.setBorderSize(0);
    add(goal2);

    goal1.setPosition(0, 60, 2, 120);
    goal1.setColor(touchgfx::Color::getColorFromRGB(0, 11, 110));
    goal1.setBorderColor(touchgfx::Color::getColorFromRGB(59, 47, 5));
    goal1.setBorderSize(0);
    add(goal1);

    ball.setPosition(154, 114, 14, 14);
    ball.setCenter(7.5f, 7.5f);
    ball.setRadius(6);
    ball.setLineWidth(0);
    ball.setArc(0, 360);
    ballPainter.setColor(touchgfx::Color::getColorFromRGB(153, 12, 12));
    ball.setPainter(ballPainter);
    add(ball);

    line1.setPosition(13, 114, 33, 19);
    line1Painter.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    line1.setPainter(line1Painter);
    line1.setStart(5, 5);
    line1.setEnd(95, 45);
    line1.setLineWidth(1);
    line1.setLineEndingStyle(touchgfx::Line::BUTT_CAP_ENDING);
    line1.setAlpha(95);
    add(line1);

    line1_1.setPosition(268, 114, 33, 19);
    line1_1Painter.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    line1_1.setPainter(line1_1Painter);
    line1_1.setStart(5, 5);
    line1_1.setEnd(95, 45);
    line1_1.setLineWidth(1);
    line1_1.setLineEndingStyle(touchgfx::Line::BUTT_CAP_ENDING);
    line1_1.setAlpha(95);
    add(line1_1);
}

MediumScreenViewBase::~MediumScreenViewBase()
{
    touchgfx::CanvasWidgetRenderer::resetBuffer();
}

void MediumScreenViewBase::setupScreen()
{

}
