#include <gui/screen1_screen/Screen1View.hpp>

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::updateRadioComActiveValue(float value)
{
    Unicode::snprintfFloat(radioComAcBuffer, RADIOCOMAC_SIZE, "%3.3f", value);
    radioComAc.invalidate();
}