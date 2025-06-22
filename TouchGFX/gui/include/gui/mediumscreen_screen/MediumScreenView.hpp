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
    void handleTickEvent();
protected:
    float ballX;      // Vị trí X của bóng
	float ballY;      // Vị trí Y của bóng
	float ballVelX;   // Vận tốc X của bóng
	float ballVelY;   // Vận tốc Y của bóng
	bool waitingForServe; // Trạng thái chờ phát bóng
	int serveDelayTicks;  // Đếm tick để delay 1 giây
	int servingPlayer;    // Người chơi phát bóng (1: paddle1, 2: paddle2)
    int score1;           // Điểm số người chơi 1
    int score2;           // Điểm số người chơi 2
    bool gameOver;        // Trạng thái trò chơi
};

#endif // MEDIUMSCREENVIEW_HPP
