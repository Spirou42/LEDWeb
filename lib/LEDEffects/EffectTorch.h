/**
EffectTorch.h
*/
#include "EffectBase.h"

#define numLeds NUM_LEDS
#define ledsPerLevel MATRIX_WIDTH
#define levels MATRIX_HEIGHT
#define USE_RED_THEME 1
//#define USE_BLUE_THEME 1


class EffectTorch: public Effect {
public:
  EffectTorch():Effect("Torch"){}

  enum {
    torch_passive = 0, // just environment, glow from nearby radiation
    torch_nop = 1, // no processing
    torch_spark = 2, // slowly looses energy, moves up
    torch_spark_temp = 3, // a spark still getting energy from the level below
  };

  inline void reduce(byte &aByte, byte aAmount, byte aMin = 0);

  inline void increase(byte &aByte, byte aAmount, byte aMax = 255);

  uint16_t random2(uint16_t aMinOrMax, uint16_t aMax = 0);

  void calcNextEnergy();

  void calcNextColors();

  void injectRandom();

  void startEffect();

  virtual void frame(unsigned long now);
  virtual uint16_t frameRate();
  virtual void initializePublicParameter();

 protected:
  const uint8_t energymap[32] = {0, 64, 96, 112, 128, 144, 152, 160, 168, 176, 184, 184, 192, 200, 200, 208, 208, 216, 216, 224, 224, 224, 232, 232, 232, 240, 240, 240, 240, 248, 248, 248};

  #ifdef USE_RED_THEME
  byte flame_min = 20; // 0..255
  byte flame_max = 220; // 0..255

  byte random_spark_probability = 5; // 0..100
  byte spark_min = 220; // 0..255
  byte spark_max = 255; // 0..255

  byte spark_tfr = 40; // 0..256 how much energy is transferred up for a spark per cycle
  uint16_t spark_cap = 195; // 0..255: spark cells: how much energy is retained from previous cycle

  uint16_t up_rad = 40; // up radiation
  uint16_t side_rad = 35; // sidewards radiation
  uint16_t heat_cap = 80; // 0..255: passive cells: how much energy is retained from previous cycle
  bool usePalette = false;

  byte red_bg = 0;
  byte green_bg = 0;
  byte blue_bg = 0;

  byte red_bias = 10;
  byte green_bias = 0;
  byte blue_bias = 0;

  int red_energy = 180;
  int green_energy = 20; // 145;
  int blue_energy = 0;
#elif USE_BLUE_THEME
  byte flame_min = 20; // 0..255
  byte flame_max = 220; // 0..255

  byte random_spark_probability = 3; // 0..100
  byte spark_min = 220; // 0..255
  byte spark_max = 255; // 0..255

  byte spark_tfr = 60; // 0..256 how much energy is transferred up for a spark per cycle
  uint16_t spark_cap = 150; // 0..255: spark cells: how much energy is retained from previous cycle

  uint16_t up_rad = 60; // up radiation
  uint16_t side_rad = 35; // sidewards radiation
  uint16_t heat_cap = 120; // 0..255: passive cells: how much energy is retained from previous cycle
  bool usePalette = false;

  byte red_bg = 0;
  byte green_bg = 0;
  byte blue_bg = 0;

  byte red_bias = 10;
  byte green_bias = 20;
  byte blue_bias = 10;

  int red_energy = 50;
  int green_energy = 50; // 145;
  int blue_energy = 180;
#endif

  uint16_t cycle_wait = 0; // 0..255

    
  byte currentEnergy[numLeds+1]; // current energy level
  byte nextEnergy[numLeds+1]; // next energy level
  byte energyMode[numLeds+1]; // mode how energy is calculated for this point

};
