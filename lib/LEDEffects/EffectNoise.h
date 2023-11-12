/**
EffectNoise.h
**/
#pragma once

#include "EffectBase.h"
#define MAX_DIMENSION ((MATRIX_WIDTH>MATRIX_HEIGHT) ? MATRIX_WIDTH : MATRIX_HEIGHT)
class EffectNoise: public Effect{
public:
  EffectNoise():Effect("Clouds"){}
  virtual uint16_t frameRate(){return 25;}

  virtual void startEffect();
  // virtual size_t numberOfParameters(){
  //   return ( _parameters.end() - _parameters.begin());
  // }
  virtual String parameterNameAt(size_t idx);
  virtual ValueWrapper* parameterAt(size_t idx);

  virtual void frame(unsigned long now);
protected:
  // The 16 bit version of our coordinates
 uint16_t x;
 uint16_t y;
 uint16_t z;
 int16_t speed = 2; // speed is set dynamically once we've started up
 int16_t scale = 50; // scale is set dynamically once we've started up
 uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

 void fillnoise8();
};
