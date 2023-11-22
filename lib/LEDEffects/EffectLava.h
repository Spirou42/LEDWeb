#ifndef __EFFECT_LAVA_H__
#define __EFFECT_LAVA_H__

#include "EffectBase.h"
#include <MetaBall.h>
#include <vector>
typedef enum { moving, atTop, atBottom } blobState_t;

class LavaBlob : public MetaBall {
public:
  LavaBlob(float x, float y, float speedX, float speedY, float radius,
           int colorIndex)
      : MetaBall(x, y, speedX, speedY, radius, colorIndex) {}

  virtual void update();
  // virtual void draw(XYMatrix &matrix);

  blobState_t updatePosition();
  float limitRadius();

  /**
   * @brief returns true if the this is touching the otherBlob
   *
   * @param otherBlob
   * @return true
   * @return false
   */
  bool isTouching(LavaBlob &otherBlob);

  bool isMoving = false;
  int someIndex;
};

class EffectLava : public Effect {
public:
  EffectLava() : Effect("Lava Lamp") {}
  virtual uint16_t frameRate() { return 30; }
  virtual void startEffect();
  virtual void frame(unsigned long now);

protected:
  int16_t numberOfBlobs;
  int16_t maxBlobSize;
  std::vector<LavaBlob *> lavaBlobs;

private:
  void createLavaBlobs(int numOfBlobs);
  void deleteLavaBlobs();
};

#endif
