/**
 * StatusLED.cpp
 * 
 */
//#define ARDUNIO_CORE_SPI

#define FASTLED_ALL_PINS_HARDWARE_SPI

#include "StatusLED.h"
#include "MatrixDemo.h"

// #define RED_CHANNEL     0
// #define GREEN_CHANNEL   1
// #define BLUE_CHANNEL    2

//extern CRGB ledBuffer[];

StatusLED LED;

StatusLED::StatusLED(){

}

// StatusLED::StatusLED(uint8_t rPin, uint8_t gPin, uint8_t bPin):_redPin(rPin),_greenPin(gPin),_bluePin(bPin){
    
// }

void StatusLED::attachPins(uint8_t rPin, uint8_t gPin, uint8_t bPin){
//    FastLED.addLeds<APA102, DATA_PIN,CLOCK_PIN,BGR,DATA_RATE_MHZ(8)>(ledBuffer,NUM_LEDS);
//    FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);
    _connectPins();
}
void StatusLED::_connectPins(){
    setColor(CRGB::Black);
}

void StatusLED::_updateColor(){
  ledMatrix.fill(_currentColor);
  ledMatrix.flush();
  //FastLED.show();
}

void StatusLED::setColor(CRGB color){
    _currentColor = color;
    _updateColor();
}

void blinker(StatusLED* t) 
{
    if(t->color() == t->_blinkColor){
      t->setColor(CRGB::Black);
    } else {
      t->setColor(t->_blinkColor);
      // t->setColor(CRGB::DarkGreen);
    }
}

void StatusLED::startBlink(CRGB color, float freq)
{
    _blinkColor = color;
    float k = 1.0/freq;
    _tBlinker.attach(k, blinker,this );
    //Serial << "Start: "<<k<<endl;
}

void StatusLED::stopBlink()
{
    _tBlinker.detach();
}