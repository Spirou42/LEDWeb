/*
EffectMetaBall.h
*/

#pragma once

#include "EffectBase.h"
#include <list>
#define MAX_BALLS 3


/* datastructure for single metaball*/
// Metaball.h

class Metaball {
public:
 Metaball(float initialX, float initialY, float initialSpeedX,
          float initialSpeedY, float initialSize, uint8_t colorIndex);

 void update();
 void draw(XYMatrix& matrix);

protected:
    float x;
    float y;
    float speedX;
    float speedY;
    float size;
    uint8_t colorIndex;

};




class EffectMetaBall:public Effect{

public:
  EffectMetaBall():Effect("MetaBall"){}
  virtual uint16_t frameRate();
  virtual void startEffect();
  virtual void frame(unsigned long now);


protected:
 std::list<Metaball*> metaballs;
 void updateMetaball(int16_t i);
 void drawMetaball(int16_t i);
};

