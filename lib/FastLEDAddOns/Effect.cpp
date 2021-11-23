/**
Effect.cpp
**/


#include "Effect.hpp"
#include <Streaming.h>
ADDONS_NAMESPACE_BEGIN
typedef void(*postFrameCallback)(unsigned long now);
volatile int16_t requestedFrameRate;

int effectRunner(unsigned long now, void* userdata){
  static EffectList::iterator lastTimeEffect = systemEffectList.end();
  static unsigned long lastTime = 0;
  static int lastDelay = 0;
  int delay = now - lastTime;
  
#if DEBUG_EFFECTS
  if(delay != lastDelay){
    Serial << "Efect: " << delay << endl;
    lastDelay = delay;
  }
#endif

  lastTime = now;
  // Serial << "EffectR:"<<(*currentRunningEffect)->name()<<endl;
  if(currentRunningEffect == systemEffectList.end()){
    currentRunningEffect = systemEffectList.begin();
    Serial << "Swap around Effects"<<endl;
  }
  if(lastTimeEffect != currentRunningEffect){
    Serial<< "Changed Effect"<<endl;
    if(lastTimeEffect != systemEffectList.end()){
      (*lastTimeEffect)->stopEffect();
      Serial<<"Stop: "<<(*lastTimeEffect)->name()<<endl;
    }
    (*currentRunningEffect)->startEffect();
    Serial << "Start: "<<(*currentRunningEffect)->name()<<endl;
    lastTimeEffect = currentRunningEffect;
    //requestedFrameRate = (*currentRunningEffect)->frameRate();
  }
  (*currentRunningEffect)->frame(now);
  requestedFrameRate = (*currentRunningEffect)->frameRate();
  if(userdata){
    postFrameCallback p = (postFrameCallback)userdata;
    p(now);
  }
  return 0;
}
ADDONS_NAMESPACE_END
