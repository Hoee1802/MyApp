#include <gui/hardscreen_screen/HardScreenView.hpp>
#include "main.h"
#include "cmsis_os.h"
#include "joystick_task.h"

extern osMessageQueueId_t joystickQueueHandle;

HardScreenView::HardScreenView():ballX(160), ballY(120), ballVelX(1.4f), ballVelY(1.4f),
								  waitingForServe(false), serveDelayTicks(0), servingPlayer(0),
								  score1(0), score2(0),gameOver(false),buzzerBeepCounter(0), buzzerBeepState(false)
{

}

void HardScreenView::setupScreen()
{
    HardScreenViewBase::setupScreen();
    score1 = 0;
	score2 = 0;
	gameOver = false;
	ballX = 160;
	ballY = 120;

	ballVelX = 1.4f;
	ballVelY = 1.4f;
	waitingForServe = false;
	serveDelayTicks = 0;
	servingPlayer = 0;
    buzzerBeepCounter = 0;
    buzzerBeepState = false;
    // Đặt vị trí ban đầu của bóng
	ball.invalidate();
	ball.moveTo(ballX, ballY);
	ball.invalidate();
    // Khởi tạo điểm số
	Unicode::snprintf(HardScreenViewBase::score1Buffer, HardScreenViewBase::SCORE1_SIZE, "%d", score1);
	HardScreenViewBase::score1.invalidate();
	Unicode::snprintf(HardScreenViewBase::score2Buffer, HardScreenViewBase::SCORE2_SIZE, "%d", score2);
	HardScreenViewBase::score2.invalidate();
}

void HardScreenView::tearDownScreen()
{
    HardScreenViewBase::tearDownScreen();
}

void HardScreenView::handleTickEvent()
{

    if (gameOver) {
        return; // Không xử lý nếu trò chơi kết thúc
    }
    // Xử lý bíp buzzer
	if (buzzerBeepCounter > 0) {
		if (buzzerBeepCounter % 6 == 0) { // Bíp mỗi 100ms (6 ticks tại 60 FPS)
			buzzerBeepState = !buzzerBeepState;
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, buzzerBeepState ? GPIO_PIN_SET : GPIO_PIN_RESET);
		}
		buzzerBeepCounter--;
		if (buzzerBeepCounter == 0) {
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET); // Tắt buzzer sau khi bíp xong
		}
	}

    JoystickCommand_t command;
    while (osMessageQueueGet(joystickQueueHandle, &command, NULL, 0) == osOK) {
        switch (command) {
            case JOY1_LEFT:
				{
					int16_t newY=paddle1.getY()+2;
					if (newY >198) {
						newY = 198; // Dừng tại cạnh dưới
					}
                    if (newY != paddle1.getY()) { // Chỉ cập nhật nếu có thay đổi
                        paddle1.invalidate();
                        paddle1.moveTo(paddle1.getX(), newY);
                        paddle1.invalidate();
                    }
				}
                break;

            case JOY1_RIGHT:
				{
					int16_t newY = paddle1.getY() - 2;
					if (newY < 2) {
						newY = 2; // Dừng tại cạnh trên
					}
					if (newY != paddle1.getY()) { // Chỉ cập nhật nếu có thay đổi
						paddle1.invalidate();
						paddle1.moveTo(paddle1.getX(), newY);
						paddle1.invalidate();
					}
				}
                break;
            case JOY2_LEFT:
				{
					int16_t newY=paddle2.getY()+2;
					if (newY >198) {
						newY = 198; // Dừng tại cạnh dưới
					}
					if (newY != paddle2.getY()) { // Chỉ cập nhật nếu có thay đổi
						paddle2.invalidate();
						paddle2.moveTo(paddle2.getX(), newY);
						paddle2.invalidate();
					}
				}
                break;
            case JOY2_RIGHT:
				{
					int16_t newY = paddle2.getY() - 2;
					if (newY < 2) {
						newY = 2; // Dừng tại cạnh trên
					}
					if (newY != paddle2.getY()) { // Chỉ cập nhật nếu có thay đổi
						paddle2.invalidate();
						paddle2.moveTo(paddle2.getX(), newY);
						paddle2.invalidate();
					}
				}
				break;
            case JOY1_BUTTON:
				// Người chơi 1 bấm nút để phát bóng
				if (waitingForServe && servingPlayer == 1) {
					waitingForServe = false;
					ballVelX = 1.4f; // Phát bóng sang phải
					ballVelY = (paddle1.getY() + paddle1.getHeight()/2 > ballY) ? -1.4f : 1.4f;
					ball.setVisible(true);
					ball.invalidate();
				}
				break;
			case JOY2_BUTTON:
				// Người chơi 2 bấm nút để phát bóng
				if (waitingForServe && servingPlayer == 2) {
					waitingForServe = false;
					ballVelX = -1.4f; // Phát bóng sang trái
					ballVelY = (paddle2.getY() + paddle2.getHeight()/2 > ballY) ? -1.4f : 1.4f;
					ball.setVisible(true);
					ball.invalidate();
				}
				break;
        }
    }

    // Cập nhật bóng nếu không chờ phát
        if (!waitingForServe) {
            ball.invalidate();
            ballX += ballVelX;
            ballY += ballVelY;

            // Xử lý va chạm với cạnh trên/dưới
            if (ballY < 0) {
                ballY = 0;
                ballVelY = -ballVelY;
            } else if (ballY + ball.getHeight() > 240) {
                ballY = 240 - ball.getHeight();
                ballVelY = -ballVelY;
            }

            // Xử lý va chạm với paddle1 (bên trái)
            if (ballX <= paddle1.getX() + paddle1.getWidth() &&
                ballX >= paddle1.getX() &&
                ballY + ball.getHeight() >= paddle1.getY() &&
                ballY <= paddle1.getY() + paddle1.getHeight()) {
                ballX = paddle1.getX() + paddle1.getWidth();
                ballVelX = -ballVelX;
            }

            // Xử lý va chạm với paddle2 (bên phải)
            if (ballX + ball.getWidth() >= paddle2.getX() &&
                ballX + ball.getWidth() <= paddle2.getX() + paddle2.getWidth() &&
                ballY + ball.getHeight() >= paddle2.getY() &&
                ballY <= paddle2.getY() + paddle2.getHeight()) {
                ballX = paddle2.getX() - ball.getWidth();
                ballVelX = -ballVelX;
            }

            // Xử lý va chạm với các image
                   bool imageCollision = false;
                   int newServingPlayer = 0;
                   // Kiểm tra va chạm với image1
                   if (ballX + ball.getWidth() >= image1.getX() &&
                       ballX <= image1.getX() + image1.getWidth() &&
                       ballY + ball.getHeight() >= image1.getY() &&
                       ballY <= image1.getY() + image1.getHeight()) {
                       imageCollision = true;
                       newServingPlayer = ballVelX > 0 ? 2 : 1; // Nếu bóng đi sang phải, người chơi 1 phát; ngược lại, người chơi 2
                   }
                   // Kiểm tra va chạm với image1_1
                   if (ballX + ball.getWidth() >= image1_1.getX() &&
                       ballX <= image1_1.getX() + image1_1.getWidth() &&
                       ballY + ball.getHeight() >= image1_1.getY() &&
                       ballY <= image1_1.getY() + image1_1.getHeight()) {
                       imageCollision = true;
                       newServingPlayer = ballVelX > 0 ? 2 : 1;
                   }
                   // Kiểm tra va chạm với image2
                   if (ballX + ball.getWidth() >= image2.getX() &&
                       ballX <= image2.getX() + image2.getWidth() &&
                       ballY + ball.getHeight() >= image2.getY() &&
                       ballY <= image2.getY() + image2.getHeight()) {
                       imageCollision = true;
                       newServingPlayer = ballVelX > 0 ? 2 : 1;
                   }
                   // Kiểm tra va chạm với image2_2
                   if (ballX + ball.getWidth() >= image2_2.getX() &&
                       ballX <= image2_2.getX() + image2_2.getWidth() &&
                       ballY + ball.getHeight() >= image2_2.getY() &&
                       ballY <= image2_2.getY() + image2_2.getHeight()) {
                       imageCollision = true;
                       newServingPlayer = ballVelX > 0 ? 2 : 1;
                   }

                   if (imageCollision) {
                       // Cộng điểm cho người chơi còn lại
                       if (newServingPlayer == 2) {
                           score2++;
                           Unicode::snprintf(HardScreenViewBase::score2Buffer, HardScreenViewBase::SCORE2_SIZE, "%d", score2);
                           HardScreenViewBase::score2.invalidate();
                           buzzerBeepCounter = 18; // 3 tiếng bíp (18 ticks = 3 * 6 ticks tại 60 FPS)
						   buzzerBeepState = true;
						   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
                           if (score2 >= 11) {
                               gameOver = true;
                               presenter->goToEndScreen(2);
                           }
                       } else {
                           score1++;
                           Unicode::snprintf(HardScreenViewBase::score1Buffer, HardScreenViewBase::SCORE1_SIZE, "%d", score1);
                           HardScreenViewBase::score1.invalidate();
                           buzzerBeepCounter = 18; // 3 tiếng bíp (18 ticks = 3 * 6 ticks tại 60 FPS)
						   buzzerBeepState = true;
						   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
                           if (score1 >= 11) {
                               gameOver = true;
                               presenter->goToEndScreen(1);
                           }
                       }
                       // Thiết lập lại trạng thái phát bóng
                       ball.setVisible(false);
                       ball.invalidate();
                       waitingForServe = true;
                       serveDelayTicks = 60; // 1 giây với 60 FPS
                       servingPlayer = newServingPlayer;
                       if (newServingPlayer == 1) {
                           ballX = paddle1.getX() + paddle1.getWidth() + 10; // Gần paddle1
                           ballY = paddle1.getY() + paddle1.getHeight()/2 - ball.getHeight()/2; // Căn giữa paddle
                       } else {
                           ballX = paddle2.getX() - ball.getWidth() - 10; // Gần paddle2
                           ballY = paddle2.getY() + paddle2.getHeight()/2 - ball.getHeight()/2; // Căn giữa paddle
                       }
                       ball.moveTo(ballX, ballY);
                       ball.invalidate();
                       return; // Thoát để không xử lý thêm
                   }

            // Xử lý va chạm với bên trái (ballX < 1)
                if (ballX >319) {
                    if (ballY > 60 && ballY < 181) {
                        // Bóng trong vùng goal2, cộng điểm cho người chơi 1
                        score1++;
                        Unicode::snprintf(HardScreenViewBase::score1Buffer, HardScreenViewBase::SCORE1_SIZE, "%d", score1);
                        HardScreenViewBase::score1.invalidate();

                        buzzerBeepCounter = 18; // 3 tiếng bíp (18 ticks = 3 * 6 ticks tại 60 FPS)
						buzzerBeepState = true;
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
                        if (score1 >= 11) {
                            gameOver = true;
                            presenter->goToEndScreen(1);
                        }
                        ball.setVisible(false);
                        ball.invalidate();
                        waitingForServe = true;
                        serveDelayTicks = 60; // 1 giây với 60 FPS
                        servingPlayer = 1; // Người chơi 1 phát bóng
                        ballX = paddle1.getX() + paddle1.getWidth() + 10; // Gần paddle1
                        ballY = paddle1.getY() + paddle1.getHeight()/2 - ball.getHeight()/2; // Căn giữa paddle
                        ball.moveTo(ballX, ballY);
                        ball.invalidate();
                        return; // Thoát để không xử lý thêm
                    } else {
                        // Bóng ngoài vùng goal, phản xạ
                        ballX = 319;
                        ballVelX = -ballVelX;
                    }
                }

                // Xử lý va chạm với bên phải (ballX > 319)
                if (ballX <1) {
                    if (ballY > 60 && ballY < 181) {
                        // Bóng trong vùng goal1, cộng điểm cho người chơi 2
                        score2++;
                        Unicode::snprintf(HardScreenViewBase::score2Buffer, HardScreenViewBase::SCORE2_SIZE, "%d", score2);
                        HardScreenViewBase::score2.invalidate();
                        buzzerBeepCounter = 18; // 3 tiếng bíp (18 ticks = 3 * 6 ticks tại 60 FPS)
						buzzerBeepState = true;
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
                        if (score2 >= 11) {
                            gameOver = true;
                            presenter->goToEndScreen(2);
                        }
                        ball.setVisible(false);
                        ball.invalidate();
                        waitingForServe = true;
                        serveDelayTicks = 60; // 1 giây với 60 FPS
                        servingPlayer = 2; // Người chơi 2 phát bóng
                        ballX = paddle2.getX() - ball.getWidth() - 10; // Gần paddle2
                        ballY = paddle2.getY() + paddle2.getHeight()/2 - ball.getHeight()/2; // Căn giữa paddle
                        ball.moveTo(ballX, ballY);
                        ball.invalidate();
                        return; // Thoát để không xử lý thêm
                    } else {
                        // Bóng ngoài vùng goal, phản xạ
                        ballX = 1;
                        ballVelX = -ballVelX;
                    }
                }


            // Di chuyển bóng
            ball.moveTo(ballX, ballY);
            ball.invalidate();
        } else {
            // Đếm thời gian chờ 1 giây
            if (serveDelayTicks > 0) {
                serveDelayTicks--;
                if (serveDelayTicks == 0) {
                    // Hiện bóng sau 1 giây
                    ball.setVisible(true);
                    ball.invalidate();
                }
            }
            // Cập nhật vị trí bóng theo paddle khi chờ phát
            if (servingPlayer == 1) {
                ballY = paddle1.getY() + paddle1.getHeight()/2 - ball.getHeight()/2;
            } else if (servingPlayer == 2) {
                ballY = paddle2.getY() + paddle2.getHeight()/2 - ball.getHeight()/2;
            }
            ball.moveTo(ballX, ballY);
            ball.invalidate();
        }
}





