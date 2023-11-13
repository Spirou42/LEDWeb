/*
EffectMetaBall.cpp
*/

#include <EffectMetaBall.h>

#define NUM_OF_BALLS 20
Metaball::Metaball(float initialX, float initialY, float initialSpeedX,
                   float initialSpeedY, float initialSize, uint8_t cidx)
    : x(initialX), y(initialY), speedX(initialSpeedX), speedY(initialSpeedY),
      size(initialSize), colorIndex(cidx) {}

void Metaball::update() {
  x += speedX;

  if (x < 0 || x >= MATRIX_WIDTH) {
    speedX = -speedX;
  }

  y += speedY;

  if (y < 0 || y >= MATRIX_HEIGHT) {
    speedY = -speedY;
  }
}

void Metaball::draw(XYMatrix &matrix) {
  // Calculate bounding box
  int minX = max(0, static_cast<int>(x - size));
  int maxX = min(matrix.width() - 1, static_cast<int>(x + size));
  int minY = max(0, static_cast<int>(y - size));
  int maxY = min(matrix.height() - 1, static_cast<int>(y + size));

  for (int i = minX; i <= maxX; i++) {
    for (int j = minY; j <= maxY; j++) {
      float dx = x - i;
      float dy = y - j;
      float distanceSquared = dx * dx + dy * dy;

      float radiusSquared = size * size;

      if (distanceSquared < radiusSquared) {
        int brightness =
            static_cast<int>(255 * (1 - distanceSquared / radiusSquared));
        CRGB dest = matrix.getPixel(i, j);
        CRGB source = ColorFromPalette((*currentSystemPalette)->second.palette,
                                       (colorIndex)*255 / NUM_OF_BALLS,
                                       brightness, LINEARBLEND);
        if (dest == CRGB::Black) {
          dest = source;
        } else {
          nblend(dest, source, 128);
        }
        matrix.setPixel(i, j, dest);
      }
    }
  }
}

void EffectMetaBall::startEffect() {
  Effect::startEffect();
  // Initialize metaballs
  metaballs.clear();
  if (metaballs.empty()) {
    for (int i = 0; i < NUM_OF_BALLS; i++) {
      Metaball *k = new Metaball(random(MATRIX_WIDTH), random(MATRIX_HEIGHT),
                                 random(1, 10) / 40.0, random(1, 10) / 40.0,
                                 random(1.0, 3.0), i);
      metaballs.push_back(k);
    }
  }
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
