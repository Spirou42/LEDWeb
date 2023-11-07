/**
* EffectWhitney.cpp
* D60 C0 B160 Q0 Z12000 U180

sequence
Q0Z1U180R400V1&20 #r400 200 20 #z1 5 60 %r #r200 30 60%r #z5 14 60 #r30 75 30 %z#z14 1 60#r75 400 60 %z
Q4Z1U180R400V1&20 #r400 200 20 #z1 5 60 %r #r200 30 60%r #z5 14 60 #r30 75 30 %z#z14 1 60#r75 400 60 %z

Q0Z1U180R400V1
Q0Z14U180R75V0
Q4Z5U180R60V0


*/
#include "MatrixDemo.h"
#include "EffectWhitney.h"


void EffectWhitney::startEffect()
{
  Effect::startEffect();
  // display.clear();
  blendFactor = 14;
  globalHueStep = 2;
  _millisSinceStart = 0;
  _frameCounter = 0;
  _lumenScale = 100;
  _cycleLength = 60 * 3;
  if (_parameters.empty()) {
    ValueWrapper *k = new ValueWrapper(&_lumenScale, 0, 400, "Luma Scale", "LSC");
    addParameter(k);
    k = new ValueWrapper(&_cycleLength, 0, 2000, "Cycle Length", "CLN");
    addParameter(k);
  }
}

void EffectWhitney::stopEffect()
{

}


void EffectWhitney::frame(unsigned long now)
{
  uint16_t numberOfPixels = MATRIX_WIDTH * MATRIX_HEIGHT;
  //Serial <<clearLineRight<<"W:"<<display.displayWidth()<<" h:"<<display.displayHeight()<<endl;
  _speed = (2*PI*numberOfPixels)/_cycleLength;
  double timer = (_millisSinceStart / 1000.0)*_speed;
  float /*minR=1000,maxR=0,*/ maxL=0,minL=1000;
  uint8_t minH=255,maxH=0;
  for (uint16_t i = 0; i < numberOfPixels; i++) {
    float r = 1 - (i/ ((float)numberOfPixels *1.0));
  //  maxR = maxR<r?r:maxR;
  //  minR = minR>r?r:minR;
    float a;
    a = timer * r;
    float lScale = (_lumenScale/50.0)-1.0;
    float hSpeed =((200-globalHueStep) -100);
    float lum = (sin(a)+lScale) / (1.0+lScale) ;
    maxL = maxL<lum?lum:maxL;
    minL = minL>lum?lum:minL;
    float hSpeedT = (100.0 + hSpeed);
    if(hSpeedT == 0.0){
      hSpeedT = 0.0;
    }else{
      hSpeedT = 1/hSpeedT;
    }
    float hueStep = 0.0;
    hueStep = (globalHueStep == 0) ? 0.0: (timer * hSpeedT);
    float hue =  (256.0-hueStep)/* *0.001 hueSpeed)*/; //(r /*+ hueStep*/);//
    uint8_t h =(uint8_t)((hue+hueStep)*255.0);//((hue-int(hue))*255);
    //Serial << h <<" ";
    maxH = maxH<h?h:maxH;
    minH = minH>h?h:minH;
    float x = (i%MATRIX_WIDTH);
    float y = (i/MATRIX_WIDTH);
    // if(i==0){
    //   Serial << ScreenPos(15,1)<<clearLineRight<<" hStep:"<<hueStep<<" ("<<hSpeedT<<")"<<endl;
    //   if (hSpeedT != 0.0) {
    //     Serial << "Ba "<<(1-hSpeedT)<<endl;
    //   }
    //   Serial.print(hSpeedT);
    // }
    CRGB c = ColorFromPalette((*currentSystemPalette)->second.palette, globalHue);
    CHSV hC = rgb2hsv(c);
    //uint8_t k = lum*255;
    // while(k>255){
    //   k-=255;
    // }
    //

    //maxL = maxL<hC.v?hC.v:maxL;
    //minL = minL>hC.v?hC.v:minL;
    hC.v*=lum;// (k);
    //maxR = maxR<hC.v?hC.v:maxR;
    //minR = minR>hC.v?hC.v:minR;

    ledMatrix.setPixel(x,y,hC);
  }
  // Serial <<"Hue: "<<minH<<" "<<maxH<<endl;
  // Serial <<"L: "<<minL<<" "<<maxL<<endl;
  // Serial <<"R: "<<minR<<" "<<maxR<<endl<<endl;

  _frameCounter++;
  ledMatrix.flush();

}

String EffectWhitney::parameterNameAt(size_t idx){
  return _parameters[idx]->getName();
}

ValueWrapper* EffectWhitney::parameterAt(size_t idx){
  return _parameters[idx];
}
#if 0
#endif