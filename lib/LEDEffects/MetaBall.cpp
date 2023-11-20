/**
 * @brief Construct a new Metaball:: Metaball object
 *
 * @param initialX
 * @param initialY
 * @param initialSpeedX
 * @param initialSpeedY
 * @param initialSize
 * @param cidx
 */
#include <EffectBase.h>
#include <MetaBall.h>
Metaball::Metaball(float initialX, float initialY, float initialSpeedX,
                   float initialSpeedY, float initialSize, int16_t colorI)
    : x(initialX), y(initialY), speedX(initialSpeedX), speedY(initialSpeedY),
      size(initialSize), colorIndex(colorI) {}

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
            static_cast<int>(255.0 * (1 - distanceSquared / radiusSquared));
        CRGB source = ColorFromPalette((*currentSystemPalette)->second.palette,
                                       colorIndex, brightness, LINEARBLEND);

        CRGB dest = matrix.getPixel(i, j);
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