/** EffectNoise.cpp
*/

#include "EffectNoise.h"
#include "EffectBase.h"

void EffectNoise::startEffect(){
  Effect::startEffect();
  blendFactor = 20;
  globalHueStep = 0;
  if(_lastPalette == systemPalettes.end()){
    currentSystemPalette = systemPalettes.begin() + PALETTE_CLOUDS;
  }
  x = random16();
  y = random16();
  z = random16();
  if (_parameters.empty()) {
    ValueWrapper *k = new ValueWrapper(&speed, 0, 100,"Speed","NSP");
    addParameter(k);
    k = new ValueWrapper(&scale,1, 300, "Scale","NSC");
    addParameter(k);
  }
}

void EffectNoise::frame(unsigned long now){
  Effect::frame(now);
  fillnoise8();
  for(int i = 0; i < ledMatrix.width(); i++) {
    for(int j = 0; j < ledMatrix.height(); j++) {
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's index into the color palette.

      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];

      //if this palette is a 'loop', add a slowly-changing base value
//      if( (*currentSystemPalette)->second.loop) {
        index += globalHue;
//      }

      // brighten up, as the color palette itself often contains the
      // light/dark dynamic range desired
      if( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }

      CRGB color = ColorFromPalette( (*currentSystemPalette)->second.palette, index, bri);
      ledMatrix.setPixel(i,j,color);
    }
  }
  ledMatrix.flush();
}

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void EffectNoise::fillnoise8() {
  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;
  if( speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }

  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;

      uint8_t data = inoise8(x + ioffset,y + joffset,z);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data,16);
      data = qadd8(data,scale8(data,39));

      if( dataSmoothing ) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }
      noise[i][j] = data;
    }
  }

  z += speed;

  // apply slow drift to X and Y, just for visual variation.
  //x += speed;
  // / 4;
  //y -= speed;
  //y -= speed / 16;
}


String EffectNoise::parameterNameAt(size_t idx){
  return _parameters[idx]->getName();
}

ValueWrapper* EffectNoise::parameterAt(size_t idx){
  return _parameters[idx];
}
