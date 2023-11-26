/**
 * @file EffectDrops.cpp
 * @author Carsten Müller (carstenm@mac.com)
 * @brief
 * @version 0.1
 * @date 2023-11-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <EffectDrops.h>

// Helpers
void swapBuffer(int16_t **t1, int16_t **t2) {
  int16_t *k = *t1;
  *t1 = *t2;
  *t2 = k;
}

void EffectDrops::startEffect() {
  Serial << "Water start" << endl;

  ledMatrix.fill(CRGB::Blue);
  damping = 25;

  dropTime = 850;
  dropStrength = 450;
  modeMask = 0;
  blendFactor = 10;
  globalHueStep = 2;
  globalHueFrameDelay = 15;
  if (_parameters.empty()) {
    ValueWrapper *k = new ValueWrapper(&dropTime, 0, 10000, "Drop Time", "DRT");
    addParameter(k);

    k = new ValueWrapper(&dropStrength, 1, 1000, "Drop Strength", "DST");
    addParameter(k);

    k = new ValueWrapper(&damping, 0, 100, "Damping", "DMP");
    addParameter(k);

    k = new ValueWrapper(&modeMask, 0, 4, "Mode", "MOD");
    addParameter(k);
  }

  dropTimer = 0;
  for (int x = 0; x < MATRIX_WIDTH; ++x) {
    for (int y = 0; y < MATRIX_HEIGHT; ++y) {
      buffer1[ledMatrix.XY(x, y)] = 0;
      buffer2[ledMatrix.XY(x, y)] = 0;
    }
  }
  b1 = buffer1;
  b2 = buffer2;
  b1[ledMatrix.XY(8, 8)] = 1000;
  //  _initPalette = Palette.currentValue();
  //  Palette.initTo(8);

  //  Serial << "Water started" << endl;
}

void EffectDrops::frame(unsigned long now) {
  // Serial << "Water Frame"<<endl;
  dropAdrop(b1);
  ProcessWater(b1, b2);
  CRGB t = CRGB::Blue;

  int16_t mode = modeMask;
  for (int x = 0; x < MATRIX_WIDTH; ++x) {
    for (int y = 0; y < MATRIX_HEIGHT; ++y) {
      int16_t k = b2[ledMatrix.XY(x, y)];

      CHSV c = (mode & 0x01) ? CHSV(globalHue, 255, 255) : rgb2hsv(CRGB::Blue);
      if (mode & 0x02) {
        uint8_t l = k & 0xff;
        c = rgb2hsv(ColorFromPalette((*currentSystemPalette)->second.palette,
                                     l /*+ globalHue*/));
      } else {
        c.value = 127;
        if (k > 0) {
          if (k > 255) {
            c.saturation = 0;
          } else {
            c.saturation = 255 - k;
          }
        }
        if (k < 0) {
          if (abs(k) > c.value) {
            c.value = 0;
          } else {
            c.value += k;
          }
        }
      }

      // CRGB color = ColorFromPalette(colorPalettes[Palette.currentValue()],k);

      // Serial << k<<" ";
      ledMatrix.setPixel(FLPoint(x, y), c);
    }
    // Serial << endl;
  }
  // Serial << endl;
  swapBuffer(&b1, &b2);

  ledMatrix.flush();
}

void EffectDrops::stopEffect() {}

String EffectDrops::parameterNameAt(size_t idx) {
  return _parameters[idx]->getName();
}

ValueWrapper *EffectDrops::parameterAt(size_t idx) { return _parameters[idx]; }

// The private ones

int16_t EffectDrops::valueAt(int16_t *source, int16_t x, int16_t y) {
  if ((x < 0) || (x > MATRIX_WIDTH - 1)) {
    return 0;
  }
  if ((y < 0) || (y > MATRIX_HEIGHT - 1)) {
    return 0;
  }
  return source[ledMatrix.XY(x, y)];
}

int16_t EffectDrops::sumNeighbours(int16_t *source, int16_t x, int16_t y) {
  int16_t result = 0, nums = 0;
  for (int i = -1; i <= 1; i++) {
    if (i == 0)
      continue;
    result += valueAt(source, x, y + i);
    nums++;
    result += valueAt(source, x + i, y);
    nums++;
  }
  result /= (nums >> 1);
  return result;
}

void EffectDrops::ProcessWater(int16_t *source, int16_t *dest) {
  // Serial << "Water process"<<endl;
  for (size_t x = 0; x < MATRIX_WIDTH; ++x) {
    for (size_t y = 0; y < MATRIX_HEIGHT; ++y) {
      size_t offset = ledMatrix.XY(x, y);
      dest[offset] = sumNeighbours(source, x, y) - dest[ledMatrix.XY(x, y)];
      dest[offset] = (float)dest[offset] * ((100.0 - (float)damping) / 100.0);
    }
  }
  // Serial << "Water processed"<<endl;
}

void EffectDrops::dropAdrop(int16_t *data) {
  //  if( random8() <= 40) {
  if (dropTimer > dropTime) {
    dropTimer = 0;
    int x = random8(MATRIX_WIDTH - 2) + 1;
    int y = random8(MATRIX_HEIGHT - 2) + 1;
    int t = 10 * dropStrength;

    uint16_t offset = ledMatrix.XY(x, y);
    data[offset] = t;
    offset = ledMatrix.XY(x + 1, y);
    if (offset < NUM_LEDS) {
      data[offset] = t;
    }
    offset = ledMatrix.XY(x + 1, y + 1);
    if (offset < NUM_LEDS) {
      data[offset] = t;
    }
  }
}
