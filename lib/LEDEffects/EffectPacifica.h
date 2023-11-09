/**
 * @file Pacifica.h
 * @author Carsten Müller (carstenm@mac.com)
 * @brief CPP variant of te Pacifica effect
 * @version 0.1
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include "EffectBase.h"
#define MAX_DIMENSION ((MATRIX_WIDTH>MATRIX_HEIGHT) ? MATRIX_WIDTH : MATRIX_HEIGHT)


class EffectPacifica : public Effect {
 public:
   EffectPacifica():Effect("Pacifica"),_twinkleSpeed(3),_twinkleDensity(3){}
   virtual void startEffect();
   virtual void stopEffect();
   virtual uint16_t frameRate(){return 1000/30;}
   virtual void frame(unsigned long now);
//   virtual size_t numberOfParameters(){return 2;}
//   virtual String parameterNameAt(size_t idx);
//   virtual ValueWrapper* parameterAt(size_t idx);

   

  protected:
   void one_layer(CRGBPalette16& p, uint16_t cistart,
                           uint16_t wavescale, uint8_t bri, uint16_t ioff);
   void add_whitecaps();
   void deepen_colors();

   CRGBPalette16 gCurrentPalette;
   CRGBPalette16 gTargetPalette;
   int16_t _twinkleSpeed;
   int16_t _twinkleDensity;
};


void pacifica_loop();

