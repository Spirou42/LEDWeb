/**
EffectLava
*/
#include "EffectLava.h"
#include <LEDEffects.h>

void EffectLava::startEffect() {
  Effect::startEffect();
  blendFactor = 10;
  globalHueStep = 0;
  numberOfBlobs = 15;
  maxBlobSize = 4;
  if (_lastPalette == systemPalettes.end()) {
    currentSystemPalette = systemPalettes.begin() + PALETTE_TEMPERATURE;
  }
  deleteLavaBlobs();

  if (lavaBlobs.empty()) {
    createLavaBlobs(numberOfBlobs);
  }
}

void EffectLava::createLavaBlobs(int nob) {
  float speedX = 0;
  float speedY = 0.05;

  for (int i = 0; i < nob; i++) {

    float radius = random(10, maxBlobSize * 10) / 10.0;
    float x = random(0, MATRIX_WIDTH);
    float y = radius;
    LavaBlob *k = new LavaBlob(x, y, speedX, speedY, radius, 170);
    k->someIndex = i;
    lavaBlobs.push_back(k);
  }
}

void EffectLava::deleteLavaBlobs() {
  for (auto it = lavaBlobs.begin(); it != lavaBlobs.end(); ++it) {
    LavaBlob *k = *it;
    delete k;
  }
  lavaBlobs.clear();
}

void EffectLava::frame(unsigned long now) {
  ledMatrix.fill(ColorFromPalette((*currentSystemPalette)->second.palette, 42,
                                  128, LINEARBLEND));
  for (auto it = lavaBlobs.begin(); it != lavaBlobs.end(); ++it) {
    (*it)->update();
    (*it)->draw(ledMatrix);
  }
  ledMatrix.flush();
}
/* LavaBlob */
void LavaBlob::update() {
  MetaBall::update();
  // Serial << "Blob: " << this->someIndex << " (" << x << ", " << y << ") ["
  //        << speedX << "," << speedY << "] " << this->radius << endl;
}

bool LavaBlob::isTouching(LavaBlob &otherBlob) {
  float dx = this->x - otherBlob.x;
  float dy = this->y - otherBlob.y;
  /* attention all calculations are squared */
  float dSquared = dx * dx + dy * dy;
  float tSize = (this->radius * this->radius);
  float oSize = (otherBlob.radius * otherBlob.radius);
  bool result = false;
  if (dSquared < tSize + oSize)
    result = true;
  return result;
}