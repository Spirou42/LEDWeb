/*
EffectMetaBall.cpp
*/

#include <EffectMetaBall.h>

#define NUM_OF_BALLS 20

void EffectMetaBall::startEffect() {
  Effect::startEffect();
  // Initialize metaballs
  metaballs.clear();
  if (metaballs.empty()) {
    createMetaBalls();
  }
}

void EffectMetaBall::stopEffect() {
  Effect::stopEffect();
  deleteMetaBalls();
}

void EffectMetaBall::createMetaBalls() {
  for (int i = 0; i < NUM_OF_BALLS; i++) {
    int16_t colorIndex = (i) * 255 / NUM_OF_BALLS;

    MetaBall *k = new MetaBall(random(MATRIX_WIDTH), random(MATRIX_HEIGHT),
                               random(1, 10) / 40.0, random(1, 10) / 40.0,
                               random(1, 3), colorIndex);
    metaballs.push_back(k);
  }
}

void EffectMetaBall::deleteMetaBalls() {
  for (auto it = metaballs.begin(); it != metaballs.end(); ++it) {
    MetaBall *t = *it;
    delete t;
  }
  metaballs.clear();
}
uint16_t EffectMetaBall::frameRate() { return 10; }

void EffectMetaBall::frame(unsigned long now) {
  // Update and draw metaballs
  ledMatrix.clear();
  for (auto it = metaballs.begin(); it != metaballs.end(); ++it) {
    (*it)->update();
    (*it)->draw(ledMatrix);
  }
  // Show the frame
  ledMatrix.flush();
}
