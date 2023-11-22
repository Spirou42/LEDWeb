/**
 * @brief A simple MetaBall class
 *
 */
#pragma once
#include <FastLEDAddOns.h>

class MetaBall {
public:
  MetaBall(float initialX, float initialY, float initialSpeedX,
           float initialSpeedY, float initialSize, int16_t colorIndex);

  virtual void update();
  virtual void draw(XYMatrix &matrix);

protected:
  float x;
  float y;
  float speedX;
  float speedY;
  float radius;
  int16_t colorIndex;
};