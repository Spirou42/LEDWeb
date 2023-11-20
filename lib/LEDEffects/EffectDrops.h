/**
 * @file EffectDrops.h
 * @author Carsten Müller (carstenm@mac.com)
 * @brief
 * @version 0.1
 * @date 2023-11-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <EffectBase.h>

class EffectDrops : public Effect {
public:
  EffectDrops() : Effect("Rain Drops"){};

  virtual void startEffect();
  virtual void stopEffect();
  virtual uint16_t frameRate() { return 30; }
  virtual void frame(unsigned long now);
  virtual ValueWrapper *parameterAt(size_t idx);
  virtual String parameterNameAt(size_t idx);

protected:
  int16_t damping;
  int16_t dropTime;
  int16_t dropStrength;
  int16_t modeMask;
  // int16_t hueBase;

private:
  /* internal effect buffers*/
  int16_t buffer1[MATRIX_WIDTH * MATRIX_HEIGHT];
  int16_t buffer2[MATRIX_WIDTH * MATRIX_HEIGHT];
  int16_t *b1, *b2;
  elapsedMillis dropTimer;
  void ProcessWater(int16_t *source, int16_t *dest);
  void dropAdrop(int16_t *data);
  int16_t valueAt(int16_t *source, int16_t x, int16_t y);
  int16_t sumNeighbours(int16_t *source, int16_t x, int16_t y);
};