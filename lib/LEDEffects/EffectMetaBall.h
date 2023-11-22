/*
EffectMetaBall.h
*/

#pragma once

#include "EffectBase.h"
#include <list>
#define MAX_BALLS 3

/* datastructure for single metaball*/
// Metaball.h

class EffectMetaBall : public Effect {

public:
  EffectMetaBall() : Effect("Balls") {}
  virtual uint16_t frameRate();
  virtual void startEffect();
  virtual void stopEffect();
  virtual void frame(unsigned long now);

protected:
  std::list<MetaBall *> metaballs;
  void updateMetaball(int16_t i);
  void drawMetaball(int16_t i);
  void createMetaBalls();
  void deleteMetaBalls();
};
