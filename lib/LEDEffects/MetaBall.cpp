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
MetaBall::MetaBall(float initialX, float initialY, float initialSpeedX,
                   float initialSpeedY, float initialSize, int16_t colorI)
    : x(initialX), y(initialY), speedX(initialSpeedX), speedY(initialSpeedY),
      radius(initialSize), colorIndex(colorI) {}

void MetaBall::update() {
  x += speedX;

  if (x < 0 || x >= MATRIX_WIDTH) {
    speedX = -speedX;
  }

  y += speedY;

  if (y < (radius) || y >= (MATRIX_HEIGHT - radius)) {
    speedY = -speedY;
  }
}

void MetaBall::draw(XYMatrix &matrix) {
  // Calculate bounding box
  int minX = max(0, static_cast<int>(x - radius));
  int maxX = min(matrix.width() - 1, static_cast<int>(x + radius));
  int minY = max(0, static_cast<int>(y - radius));
  int maxY = min(matrix.height() - 1, static_cast<int>(y + radius));

  for (int i = minX; i <= maxX; i++) {
    for (int j = minY; j <= maxY; j++) {
      float dx = x - i;
      float dy = y - j;
      float distanceSquared = dx * dx + dy * dy;

      float radiusSquared = radius * radius;

      if (distanceSquared < radiusSquared) {
        int brightness =
            static_cast<int>(255.0 * (1 - distanceSquared / radiusSquared));
        CRGB source = ColorFromPalette((*currentSystemPalette)->second.palette,
                                       colorIndex, brightness, LINEARBLEND);

        CRGB dest = matrix.getPixel(i, j);
        CRGB newColor;
        if (dest == CRGB::Black) {
          newColor = source;
        } else {
          newColor = blend(dest, source, 128);
        }
        matrix.setPixel(i, j, newColor);
      }
    }
  }
}