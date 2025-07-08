#include <gui/easyscreen_screen/EasyScreenView.hpp> // Header của lớp EasyScreenView, chứa định nghĩa giao diện
#include "main.h"                                  // Header chính cho STM32 HAL và cấu hình ngoại vi
#include "cmsis_os.h"                              // API CMSIS-RTOS cho tích hợp FreeRTOS
#include "joystick_task.h"                         // Header chứa định nghĩa các lệnh joystick (JoystickCommand_t)

// Handle hàng đợi tin nhắn FreeRTOS cho các lệnh từ joystick
extern osMessageQueueId_t joystickQueueHandle;

// Hàm khởi tạo của lớp EasyScreenView
EasyScreenView::EasyScreenView() : ballX(160), ballY(80), ballVelX(1), ballVelY(1), // // Khởi tạo tọa độ và vận tốc ban đầu của bóng (giữa màn hình 320x240)
                                 waitingForServe(false), serveDelayTicks(0), servingPlayer(0), // Trạng thái chờ phát bóng
                                 score1(0), score2(0), gameOver(false),                      // Điểm số và trạng thái trò chơi
                                 buzzerBeepCounter(0), buzzerBeepState(false)                // Quản lý âm thanh buzzer
{
    // Hàm khởi tạo mặc định, đặt các giá trị ban đầu cho trò chơi
}

// Phương thức thiết lập màn hình khi bắt đầu
void EasyScreenView::setupScreen()
{
    EasyScreenViewBase::setupScreen(); // Gọi phương thức thiết lập của lớp cơ sở
    // Khởi tạo lại trạng thái trò chơi
    score1 = 0;                       // Điểm số người chơi 1
    score2 = 0;                       // Điểm số người chơi 2
    gameOver = false;                 // Trò chơi chưa kết thúc
    ballX = 160;                      // Tọa độ X ban đầu của bóng (giữa màn hình)
    ballY = 120;                      // Tọa độ Y ban đầu của bóng (giữa màn hình)
    ballVelX = 1;                     // Vận tốc X ban đầu của bóng
    ballVelY = 1;                     // Vận tốc Y ban đầu của bóng
    waitingForServe = false;          // Không chờ phát bóng
    serveDelayTicks = 0;              // Bộ đếm thời gian chờ phát bóng
    servingPlayer = 0;                // Người phát bóng ban đầu (chưa xác định)
    buzzerBeepCounter = 0;            // Bộ đếm cho âm thanh buzzer
    buzzerBeepState = false;          // Trạng thái buzzer ban đầu (tắt)

    // Đặt vị trí ban đầu của bóng trên màn hình
    ball.invalidate();                // Yêu cầu vẽ lại vùng chứa bóng
    ball.moveTo(ballX, ballY);        // Di chuyển bóng đến tọa độ ban đầu
    ball.invalidate();                // Yêu cầu vẽ lại sau khi di chuyển

    // Khởi tạo hiển thị điểm số trên giao diện
    Unicode::snprintf(EasyScreenViewBase::score1Buffer, EasyScreenViewBase::SCORE1_SIZE, "%d", score1); // Chuyển điểm số 1 thành chuỗi
    EasyScreenViewBase::score1.invalidate(); // Yêu cầu vẽ lại vùng hiển thị điểm số 1
    Unicode::snprintf(EasyScreenViewBase::score2Buffer, EasyScreenViewBase::SCORE2_SIZE, "%d", score2); // Chuyển điểm số 2 thành chuỗi
    EasyScreenViewBase::score2.invalidate(); // Yêu cầu vẽ lại vùng hiển thị điểm số 2
}

// Phương thức dọn dẹp khi thoát màn hình
void EasyScreenView::tearDownScreen()
{
    EasyScreenViewBase::tearDownScreen(); // Gọi phương thức dọn dẹp của lớp cơ sở
}

// Phương thức xử lý sự kiện tick (gọi định kỳ, thường 60 FPS)
void EasyScreenView::handleTickEvent()
{
    if (gameOver) {
        return; // Thoát ngay nếu trò chơi đã kết thúc
    }

    // Xử lý âm thanh buzzer (bíp khi ghi điểm hoặc va chạm)
    if (buzzerBeepCounter > 0) {
        if (buzzerBeepCounter % 6 == 0) { // Bíp mỗi 100ms (6 ticks tại 60 FPS)
            buzzerBeepState = !buzzerBeepState; // Đảo trạng thái buzzer
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, buzzerBeepState ? GPIO_PIN_SET : GPIO_PIN_RESET); // Điều khiển chân GPIO cho buzzer
        }
        buzzerBeepCounter--; // Giảm bộ đếm
        if (buzzerBeepCounter == 0) {
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET); // Tắt buzzer khi hoàn tất
        }
    }

    // Xử lý các lệnh joystick từ hàng đợi
    JoystickCommand_t command;
    while (osMessageQueueGet(joystickQueueHandle, &command, NULL, 0) == osOK) { // Đọc tất cả lệnh trong hàng đợi
        switch (command) {
            case JOY1_LEFT: // Joystick 1 di chuyển xuống
            {
                int16_t newY = paddle1.getY() + 2; // Di chuyển thanh chắn 1 xuống 2 pixel
                if (newY > 198) {
                    newY = 198; // Giới hạn ở cạnh dưới (màn hình 240px, trừ kích thước paddle)
                }
                if (newY != paddle1.getY()) { // Chỉ cập nhật nếu vị trí thay đổi
                    paddle1.invalidate();     // Yêu cầu vẽ lại vùng cũ
                    paddle1.moveTo(paddle1.getX(), newY); // Di chuyển paddle
                    paddle1.invalidate();     // Yêu cầu vẽ lại vùng mới
                }
            }
            break;

            case JOY1_RIGHT: // Joystick 1 di chuyển lên
            {
                int16_t newY = paddle1.getY() - 2; // Di chuyển thanh chắn 1 lên 2 pixel
                if (newY < 2) {
                    newY = 2; // Giới hạn ở cạnh trên
                }
                if (newY != paddle1.getY()) { // Chỉ cập nhật nếu vị trí thay đổi
                    paddle1.invalidate();     // Yêu cầu vẽ lại vùng cũ
                    paddle1.moveTo(paddle1.getX(), newY); // Di chuyển paddle
                    paddle1.invalidate();     // Yêu cầu vẽ lại vùng mới
                }
            }
            break;

            case JOY2_LEFT: // Joystick 2 di chuyển xuống
            {
                int16_t newY = paddle2.getY() + 2; // Di chuyển thanh chắn 2 xuống 2 pixel
                if (newY > 198) {
                    newY = 198; // Giới hạn ở cạnh dưới
                }
                if (newY != paddle2.getY()) { // Chỉ cập nhật nếu vị trí thay đổi
                    paddle2.invalidate();     // Yêu cầu vẽ lại vùng cũ
                    paddle2.moveTo(paddle2.getX(), newY); // Di chuyển paddle
                    paddle2.invalidate();     // Yêu cầu vẽ lại vùng mới
                }
            }
            break;

            case JOY2_RIGHT: // Joystick 2 di chuyển lên
            {
                int16_t newY = paddle2.getY() - 2; // Di chuyển thanh chắn 2 lên 2 pixel
                if (newY < 2) {
                    newY = 2; // Giới hạn ở cạnh trên
                }
                if (newY != paddle2.getY()) { // Chỉ cập nhật nếu vị trí thay đổi
                    paddle2.invalidate();     // Yêu cầu vẽ lại vùng cũ
                    paddle2.moveTo(paddle2.getX(), newY); // Di chuyển paddle
                    paddle2.invalidate();     // Yêu cầu vẽ lại vùng mới
                }
            }
            break;

            case JOY1_BUTTON: // Nút bấm Joystick 1
            // Người chơi 1 nhấn nút để phát bóng
            if (waitingForServe && servingPlayer == 1) {
                waitingForServe = false; // Kết thúc trạng thái chờ
                ballVelX = 1; // Phát bóng sang phải
                ballVelY = (paddle1.getY() + paddle1.getHeight()/2 > ballY) ? -1 : 1; // Vận tốc Y dựa trên vị trí paddle
                ball.setVisible(true); // Hiển thị bóng
                ball.invalidate();     // Yêu cầu vẽ lại bóng
            }
            break;

            case JOY2_BUTTON: // Nút bấm Joystick 2
            // Người chơi 2 nhấn nút để phát bóng
            if (waitingForServe && servingPlayer == 2) {
                waitingForServe = false; // Kết thúc trạng thái chờ
                ballVelX = -1; // Phát bóng sang trái
                ballVelY = (paddle2.getY() + paddle2.getHeight()/2 > ballY) ? -1 : 1; // Vận tốc Y dựa trên vị trí paddle
                ball.setVisible(true); // Hiển thị bóng
                ball.invalidate();     // Yêu cầu vẽ lại bóng
            }
            break;
        }
    }

    // Cập nhật vị trí bóng nếu không chờ phát
    if (!waitingForServe) {
        ball.invalidate(); // Yêu cầu vẽ lại vùng cũ của bóng
        ballX += ballVelX; // Cập nhật tọa độ X
        ballY += ballVelY; // Cập nhật tọa độ Y

        // Xử lý va chạm với cạnh trên/dưới màn hình
        if (ballY < 0) {
            ballY = 0;           // Giới hạn ở cạnh trên
            ballVelY = -ballVelY; // Đảo vận tốc Y
        } else if (ballY + ball.getHeight() > 240) {
            ballY = 240 - ball.getHeight(); // Giới hạn ở cạnh dưới
            ballVelY = -ballVelY;           // Đảo vận tốc Y
        }

        // Xử lý va chạm với paddle1 (bên trái)
        if (ballX <= paddle1.getX() + paddle1.getWidth() &&
            ballX >= paddle1.getX() &&
            ballY + ball.getHeight() >= paddle1.getY() &&
            ballY <= paddle1.getY() + paddle1.getHeight()) {
            ballX = paddle1.getX() + paddle1.getWidth(); // Đặt bóng ngay ngoài paddle
            ballVelX = -ballVelX;                        // Đảo vận tốc X
        }

        // Xử lý va chạm với paddle2 (bên phải)
        if (ballX + ball.getWidth() >= paddle2.getX() &&
            ballX + ball.getWidth() <= paddle2.getX() + paddle2.getWidth() &&
            ballY + ball.getHeight() >= paddle2.getY() &&
            ballY <= paddle2.getY() + paddle2.getHeight()) {
            ballX = paddle2.getX() - ball.getWidth(); // Đặt bóng ngay ngoài paddle
            ballVelX = -ballVelX;                     // Đảo vận tốc X
        }

        // Xử lý ghi điểm
        if (ballX < 0) {
            // Người chơi 2 ghi điểm
            score2++; // Tăng điểm
            Unicode::snprintf(EasyScreenViewBase::score2Buffer, EasyScreenViewBase::SCORE2_SIZE, "%d", score2); // Cập nhật chuỗi điểm số
            EasyScreenViewBase::score2.invalidate(); // Yêu cầu vẽ lại điểm số
            buzzerBeepCounter = 18; // 3 tiếng bíp (18 ticks = 3 * 6 ticks tại 60 FPS)
            buzzerBeepState = true; // Bật buzzer
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET); // Bật chân GPIO cho buzzer

            if (score2 >= 11) {
                gameOver = true; // Kết thúc trò chơi
                presenter->goToEndScreen(2); // Chuyển sang màn hình kết thúc, người chơi 2 thắng
            }

            ball.setVisible(false); // Ẩn bóng
            ball.invalidate();     // Yêu cầu vẽ lại
            waitingForServe = true; // Chờ phát bóng
            serveDelayTicks = 60;  // Chờ 1 giây (60 ticks tại 60 FPS)
            servingPlayer = 2;     // Người chơi 2 phát bóng
            ballX = paddle2.getX() - ball.getWidth() - 10; // Đặt bóng gần paddle2
            ballY = paddle2.getY() + paddle2.getHeight()/2 - ball.getHeight()/2; // Căn giữa paddle
            ball.moveTo(ballX, ballY); // Di chuyển bóng
        } else if (ballX + ball.getWidth() > 320) {
            // Người chơi 1 ghi điểm
            score1++; // Tăng điểm
            Unicode::snprintf(EasyScreenViewBase::score1Buffer, EasyScreenViewBase::SCORE1_SIZE, "%d", score1); // Cập nhật chuỗi điểm số
            EasyScreenViewBase::score1.invalidate(); // Yêu cầu vẽ lại điểm số
            buzzerBeepCounter = 18; // 3 tiếng bíp (18 ticks = 3 * 6 ticks tại 60 FPS)
            buzzerBeepState = true; // Bật buzzer
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET); // Bật chân GPIO cho buzzer

            if (score1 >= 11) {
                gameOver = true; // Kết thúc trò chơi
                presenter->goToEndScreen(1); // Chuyển sang màn hình kết thúc, người chơi 1 thắng
            }

            ball.setVisible(false); // Ẩn bóng
            ball.invalidate();     // Yêu cầu vẽ lại
            waitingForServe = true; // Chờ phát bóng
            serveDelayTicks = 60;  // Chờ 1 giây (60 ticks tại 60 FPS)
            servingPlayer = 1;     // Người chơi 1 phát bóng
            ballX = paddle1.getX() + paddle1.getWidth() + 10; // Đặt bóng gần paddle1
            ballY = paddle1.getY() + paddle1.getHeight()/2 - ball.getHeight()/2; // Căn giữa paddle
            ball.moveTo(ballX, ballY); // Di chuyển bóng
        }

        // Di chuyển bóng đến vị trí mới
        ball.moveTo(ballX, ballY);
        ball.invalidate(); // Yêu cầu vẽ lại bóng
    } else {
        // Xử lý khi chờ phát bóng
        if (serveDelayTicks > 0) {
            serveDelayTicks--; // Giảm bộ đếm thời gian chờ
            if (serveDelayTicks == 0) {
                // Hiện bóng sau 1 giây
                ball.setVisible(true);
                ball.invalidate(); // Yêu cầu vẽ lại bóng
            }
        }
        // Cập nhật vị trí bóng theo paddle khi chờ phát
        if (servingPlayer == 1) {
            ballY = paddle1.getY() + paddle1.getHeight()/2 - ball.getHeight()/2; // Căn giữa paddle1
        } else if (servingPlayer == 2) {
            ballY = paddle2.getY() + paddle2.getHeight()/2 - ball.getHeight()/2; // Căn giữa paddle2
        }
        ball.moveTo(ballX, ballY); // Di chuyển bóng
        ball.invalidate();         // Yêu cầu vẽ lại bóng
    }
}
