/*
EffectTorch.cpp
*/


#include "EffectTorch.h"

void EffectTorch::startEffect()
  {
    Effect::startEffect();
    currentSystemPalette = systemPalettes.begin() + PALETTE_SUNSET;
    blendFactor = 50;

    for (int i=0; i<numLeds; i++) {
      currentEnergy[i] = 0;
      nextEnergy[i] = 0;
      energyMode[i] = torch_passive;
    }
    // #if USE_RED_THEME
    // ledMatrix.setFlipped(false);
    // #elif USE_BLUE_THEME
    // ledMatrix.setFlipped(false);
    // #endif
  }
  
  void EffectTorch::frame(unsigned long now) {
    injectRandom();
    calcNextEnergy();
    calcNextColors();
    ledMatrix.flush();
  }
  uint16_t EffectTorch::frameRate() { return 1000 / 30; }

  void EffectTorch::initializePublicParameter(){
    Serial << "Rain Parameter called" << endl;
  }


  enum {
    torch_passive = 0, // just environment, glow from nearby radiation
    torch_nop = 1, // no processing
    torch_spark = 2, // slowly looses energy, moves up
    torch_spark_temp = 3, // a spark still getting energy from the level below
  };

  inline void EffectTorch::reduce(byte &aByte, byte aAmount, byte aMin )
  {
    int r = aByte-aAmount;
    if (r<aMin)
    aByte = aMin;
    else
    aByte = (byte)r;
  }
  

  void EffectTorch::increase(byte &aByte, byte aAmount, byte aMax)
  {
    int r = aByte+aAmount;
    if (r>aMax)
    aByte = aMax;
    else
    aByte = (byte)r;
  }

  uint16_t EffectTorch::random2(uint16_t aMinOrMax, uint16_t aMax)
  {
    if (aMax==0) {
      aMax = aMinOrMax;
      aMinOrMax = 0;
    }
    uint32_t r = aMinOrMax;
    aMax = aMax - aMinOrMax + 1;
    r += rand() % aMax;
    return r;
  }

  void EffectTorch::calcNextEnergy()
  {
    int i = 0;
    for (int y=0; y<levels; y++) {
      for (int x=0; x<ledsPerLevel; x++) {
        i = ledMatrix.XY(x,y);
        byte e = currentEnergy[i];
        byte m = energyMode[i];
        switch (m) {
          case torch_spark: {
            // loose transfer up energy as long as the is any
            reduce(e, spark_tfr);
            // cell above is temp spark, sucking up energy from this cell until empty
            if (y<(levels-1)) {
              energyMode[ledMatrix.XY(x,y+1)] = torch_spark_temp;
            }
            break;
          }
          case torch_spark_temp: {
            // just getting some energy from below
            byte e2 = currentEnergy[ledMatrix.XY(x,y-1)];
            if (e2<spark_tfr) {
              // cell below is exhausted, becomes passive
              energyMode[ledMatrix.XY(x,y-1)] = torch_passive;
              // gobble up rest of energy
              increase(e, e2);
              // loose some overall energy
              e = ((int)e*spark_cap)>>8;
              // this cell becomes active spark
              energyMode[i] = torch_spark;
            } else {
              increase(e, spark_tfr);
            }
            break;
          }
          case torch_passive: {
            e = ((int)e*heat_cap)>>8;
            increase(e, ((((int)currentEnergy[ledMatrix.XY(x-1,y)]+
            (int)currentEnergy[ledMatrix.XY(x+1,y)])*side_rad)>>9) +
            (((int)currentEnergy[ledMatrix.XY(x,y-1)]*up_rad)>>8));
          }
          default:
          break;
        }
        nextEnergy[i] = e;
      }
    }
  }



  void EffectTorch::calcNextColors(){
    for(int x=0;x<MATRIX_WIDTH;x++){
      for(int y=0;y<MATRIX_HEIGHT;y++){
        uint16_t p = ledMatrix.XY(x,y);
        uint16_t e = nextEnergy[p];
        currentEnergy[p] = e;
        CRGB color;
        if(usePalette){
          color = ColorFromPalette((*currentSystemPalette)->second.palette,e+globalHue);
        }else{
          if (e>245){
            color = CRGB(250, 200, 2); // blueish extra-bright spark
          } else if (e>0) {
            //energy to brightness is non-linear
            byte eb = energymap[e>>3];
            byte r = red_bias;
            byte g = green_bias;
            byte b = blue_bias;
            increase(r, (eb*red_energy)>>8);
            increase(g, (eb*green_energy)>>8);
            increase(b, (eb*blue_energy)>>8);
            color = CRGB(r, g, b);
          } else {
            // background, no energy
            color = CRGB::Black;//(red_bg, green_bg, blue_bg);
          }
        }
        ledMatrix.setPixel(x,y,color);
      }
    }
  }


  void EffectTorch::injectRandom(){
    // random flame energy at bottom row
    for (int i=0; i<ledsPerLevel; i++) {
      int p = ledMatrix.XY(i,0);
      currentEnergy[p] = random2(flame_min, flame_max);
      energyMode[p] = torch_nop;
    }
    // random sparks at second row
    for (int i=0; i<ledsPerLevel; i++) {
      int p = ledMatrix.XY(i,1);
      if (energyMode[p]!=torch_spark && random2(100)<random_spark_probability) {
        currentEnergy[p] = random2(spark_min, spark_max);
        energyMode[p] = torch_spark;
      }
    }
  }

  
