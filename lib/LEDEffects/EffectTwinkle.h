/**
 * @file EffectTwinkle.h
 * @author Carsten Müller (carstenm@mac.com)
 * @brief Twinkle effect
 * @version 0.1
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "MatrixDemo.h"
#define MAX_DIMENSION ((MATRIX_WIDTH>MATRIX_HEIGHT) ? MATRIX_WIDTH : MATRIX_HEIGHT)

class EffectTwinkle : public Effect {
 public:
   EffectTwinkle():Effect("Twinkle"),_twinkleSpeed(3),_twinkleDensity(3){}
   virtual void startEffect();
   virtual void stopEffect();
   virtual uint16_t frameRate(){return 1000/30;}
   virtual void frame(unsigned long now);
//   virtual size_t numberOfParameters(){return 2;}
   virtual String parameterNameAt(size_t idx);
   virtual ValueWrapper* parameterAt(size_t idx);

   

  protected:

    void chooseNextColorPalette(CRGBPalette16& pal);
    void drawTwinkles(unsigned long now);
    CRGB computeOneTwinkle(uint32_t ms, uint8_t salt);
    uint8_t attackDecayWave8(uint8_t i);
    void coolLikeIncandescent(CRGB& c, uint8_t phase);

    CRGBPalette16 gCurrentPalette;
    CRGBPalette16 gTargetPalette;
    int16_t _twinkleSpeed;
    int16_t _twinkleDensity;
};
