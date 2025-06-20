#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
ModelListener* Model::staticListener = nullptr;
Model::Model() : modelListener(0)
{

}

void Model::tick()
{
    // Tick xử lý định kỳ nếu cần
    staticListener = modelListener; // gán mỗi lần để sync
}

void Model::movePaddle1Left() {
    if (staticListener) staticListener->movePaddle1Left();
}

void Model::movePaddle1Right() {
    if (staticListener) staticListener->movePaddle1Right();
}


void Model::movePaddle2Left() {
    if (staticListener) staticListener->movePaddle2Left();
}

void Model::movePaddle2Right() {
    if (staticListener) staticListener->movePaddle2Right();
}

void Model::paddle1Action() {
    if (staticListener) staticListener->paddle1Action();
}

void Model::paddle2Action() {
    if (staticListener) staticListener->paddle2Action();
}
