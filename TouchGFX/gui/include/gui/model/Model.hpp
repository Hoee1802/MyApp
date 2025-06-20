#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;


class Model
{
    
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
    // Gọi từ joystick_task.c
    static void movePaddle1Left();
    static void movePaddle1Right();

    static void movePaddle2Left();
    static void movePaddle2Right();
    static void paddle1Action();
    static void paddle2Action();
    
protected:
    ModelListener* modelListener;
    static ModelListener* staticListener; // để truy cập từ C
};



#endif // MODEL_HPP
