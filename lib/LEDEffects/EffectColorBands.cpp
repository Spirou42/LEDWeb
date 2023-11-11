/**
EffectColorBands.cpp
*/

#include <EffectColorBands.h>

void EffectColorBands::startEffect(){
  Effect::startEffect();
  blendFactor = 1;
  if (_lastPalette == systemPalettes.end()) {
    currentSystemPalette = systemPalettes.begin() + PALETTE_FOREST;
  }
   globalHueStep = 5;
   globalHueFrameDelay = 15;
   _bandWidth = 1;
   _bandColorDiff = 35;
   
   if (_parameters.empty()) {
    ValueWrapper *wrapper = new ValueWrapper(
        &_bandWidth, 1, 10, "Band Height", "CBH");
    addParameter(wrapper);

    wrapper = new ValueWrapper(&_bandColorDiff, 0, 255, "Color Step", "BCS");
    addParameter(wrapper);

    wrapper = new ValueWrapper(&_verticalBands, 0, 1, "Vertical", "VCB");
    addParameter(wrapper);

    wrapper = new ValueWrapper(&_direction, 0, 1, "Direction", "AND");
    addParameter(wrapper);
   }
}

uint16_t EffectColorBands::frameRate() { return 30; }

void EffectColorBands::frame(unsigned long now) {
  Effect::frame(now);
  // Serial << "Frame: "<<endl;
  ledMatrix.fadeToBlack(1);
  ledMatrix.fill(CRGB::Black);
  uint8_t currentHue = globalHue;
  size_t lineLimit = (_verticalBands == 0) ? MATRIX_HEIGHT : MATRIX_WIDTH;
  FLPoint s, e;
  for (uint16_t k = 0; k < lineLimit; k++) {
    if(_verticalBands){
      s = FLPoint(k, 0);
      e = FLPoint(k, MATRIX_HEIGHT - 1);
    }else{
      s = FLPoint(0, k);
      e = FLPoint(MATRIX_WIDTH - 1, k);
    }

    CRGB color =
        ColorFromPalette((*currentSystemPalette)->second.palette, currentHue);
    ledMatrix.line(s, e, color);
    if( (k>0) &&(k %_bandWidth) == 0){
      if(_direction)
        currentHue -= _bandColorDiff;
      else
        currentHue += _bandColorDiff;
    }
  }
  ledMatrix.flush();
}
String EffectColorBands::parameterNameAt(size_t idx){
  return _parameters[idx]->getName();
}

ValueWrapper* EffectColorBands::parameterAt(size_t idx){
  return _parameters[idx];
}
