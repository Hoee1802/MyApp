#include <gui/easyreadyscreen_screen/EasyReadyScreenView.hpp>

EasyReadyScreenView::EasyReadyScreenView()

{
	countdownValue = 3; // Giá trị đếm ngược ban đầu
	tickCounter = 0;     // Khởi tạo tickCounter
}

void EasyReadyScreenView::setupScreen()
{
    EasyReadyScreenViewBase::setupScreen();
    updateCounter(countdownValue);
}

void EasyReadyScreenView::tearDownScreen()
{
    EasyReadyScreenViewBase::tearDownScreen();
}
void EasyReadyScreenView::handleTickEvent()
{
	tickCounter++;
	    // Cập nhật đếm ngược mỗi 1 giây (giả sử 60 tick ~ 1 giây)
	    if (tickCounter >= 60)
	    {
	        countdownValue--;
	        updateCounter(countdownValue);
	        tickCounter = 0; // Reset tickCounter
	    }
	    if(countdownValue<=0){
	    	application().goto
	    }

void EasyReadyScreenView::updateCounter(int value)
{
    countdownValue = value;
    // Cập nhật giá trị cho textCounter1
    Unicode::snprintf(counter1Buffer, COUNTER1_SIZE, "%d", countdownValue);
    counter1.invalidate(); // Yêu cầu vẽ lại textCounter1

    // Cập nhật giá trị cho textCounter2
    Unicode::snprintf(counter2Buffer, COUNTER2_SIZE, "%d", countdownValue);
    counter2.invalidate(); // Yêu cầu vẽ lại textCounter2
}
