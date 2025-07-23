#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

extern "C" {
    extern float radioComActiveValue;
}

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
#ifndef SIMULATOR
    if(lastRadioComActiveValue != radioComActiveValue)
    {
        modelListener->notifyRadioComActiveValueChanged(radioComActiveValue);
        lastRadioComActiveValue = radioComActiveValue;
    }
#endif /* SIMULATOR */
}
