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
    
private:
    float lastRadioComActiveValue = 0.0f;
protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
