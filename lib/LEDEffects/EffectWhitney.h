/**
* EffectWhitney.h
*/
#if 1
#ifndef __EFFECT_WHITNEY_H__
#define __EFFECT_WHITNEY_H__
#include "MatrixDemo.h"

class EffectWhitney:public Effect
{
public:
  
  EffectWhitney():Effect("Drift"),_cycleLength(60*3),_lumenScale(75),_hueSpeed(2){};
  EffectWhitney(int16_t pa,int16_t ls,int16_t hs):Effect("Drift"),_cycleLength(pa),_lumenScale(ls),_hueSpeed(hs){};
  virtual void startEffect();
  virtual void frame(unsigned long now);
  virtual void stopEffect();
   virtual String parameterNameAt(size_t idx);
   virtual ValueWrapper* parameterAt(size_t idx);


  //virtual void printParameter(Print& stream);
  // virtual size_t numberOfParameters(){return 3;};
  // virtual Parameter16_t* parameterAt(size_t idx){
  //   Parameter16_t* result = NULL;
  //   switch(idx){
  //     case 0: result = cycleLength; break;
  //     case 1: result = lumenScale; break;
  //     case 2: result = hueSpeed; break;
  //     default: result = NULL; break;
  //   }
  //   return result;
  // }
  // virtual const char * parameterNameAt(size_t idx){
  //   const char * result = NULL;
  //   switch(idx){
  //     case 0: result = "cycleL"; break;
  //     case 1: result = "lumenS"; break;
  //     case 2: result = "hueSpeed"; break;
  //     default: result = NULL; break;
  //   }
  //   return result;
  // }
protected:
  int16_t _cycleLength;
  int16_t _lumenScale;
  int16_t _hueSpeed;

  elapsedMillis _millisSinceStart;
  unsigned long _frameCounter;
  float _speed;
};
#endif
#endif
