#ifndef __EFECT_LINEBOUNCE_H__
#define __EFECT_LINEBOUNCE_H__

#include "MatrixDemo.h"

class EffectLineBounce : public Effect {
public:

  EffectLineBounce():Effect("Solid"),line(0),delayedFrame(0),step(1){};

  virtual void startEffect(){
    Effect::startEffect();
    blendFactor = 6;
    if(_lastPalette == systemPalettes.end()){
      currentSystemPalette = systemPalettes.begin() + PALETTE_FOREST;
    }
    globalHueStep = 5;
    globalHueFrameDelay = 10;
  }
  virtual uint16_t frameRate(){return 10;}
  virtual void frame(unsigned long now) {
    Effect::frame(now);
    //Serial << "Frame: "<<endl;
    //ledMatrix.fadeToBlack(1);
    //ledMatrix.fill(CRGB::Black);
    FLPoint s(0,line);
    FLPoint e(MATRIX_WIDTH-1,line);
    
    CRGB color =  ColorFromPalette((*currentSystemPalette)->second.palette, currentHue);

    ledMatrix.line(s,e,color);
    ledMatrix.flush();

    delayedFrame++;
    int16_t upperLine = MATRIX_HEIGHT;
    if(delayedFrame > 0){
      delayedFrame = 0;
      line +=step;
      currentHue+=globalHueStep;
      if(currentHue>255){
        currentHue = 0;
      }
      // line = line % 9;
      if (line >=upperLine){
        line = 0;
        startHue -= globalHueStep;
        if(startHue>255){
          startHue = 0;
        }
        if(startHue<0){
          startHue = 255;
        }
        currentHue = startHue;
        //step = -1;
      }else if (line <=0){
        step = 1;
        line = 0;
      }
    }
  }

protected:
  int line = 0;
  int delayedFrame = 0;
  int16_t step = 1;
  int16_t currentHue = 0;
  int16_t startHue = 0;
};

#endif
