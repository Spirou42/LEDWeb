/**
 * StatusLED
 * Simple RGB Status LED wired to 3 GIPOs
 * 
 */

#pragma once
#include <Arduino.h>
#include <Streaming.h>
#include <SPI.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <Ticker.h>
#include <FastLEDAddOns.h>
#include <MatrixDemo.h>

class StatusLED;
extern void blinker(StatusLED* led);


class StatusLED {

public:
//    StatusLED(uint8_t rPin, uint8_t gPin, uint8_t bPin);
    StatusLED();
    void attachPins(uint8_t rPin, uint8_t gPin, uint8_t bPin);
    void setColor(CRGB color);
    CRGB color() {return _currentColor;}
    void startBlink(CRGB color,float freq);
    void stopBlink();
protected:
    Ticker _tBlinker;
    CRGB _currentColor;
    CRGB _blinkColor;
    void _updateColor();
    void _connectPins();
friend void blinker(StatusLED*);
};

extern StatusLED LED;
