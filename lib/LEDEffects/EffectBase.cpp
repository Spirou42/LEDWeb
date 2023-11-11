/**
 * @file EffectBase.cpp
 * @author Carsten Müller (carstenm@mac.com)
 * @brief common and base structures for running effects. Defines global parameters, paletes and mapping for the LED matrix as defined in LasLEDAddOns.
 * @version 0.1
 * @date 2021-09-29
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <EffectBase.h>
#include <Effects.h>

#if USE_CYLINDERLAMP
#warning CYLINDER
#define FLIPPED false
#define SERPENTIME false
#define ROTATED false
#elif USE_CHRISTMASBALL
#warning CHRISTMASBALL
#define FLIPPED false
#define SERPENTIME true
#define ROTATED true
#endif

using namespace FastLEDAddOns;

/** some global parameters  later with value wrapper */

/** factor used to blend LEDs to the backbuffer */
int16_t blendFactor = 20;


/** parapertes for HUE animation */
uint8_t globalHue = 0 ;                     /// current Hue Offset internal global parameter 
int16_t globalHueStep = 1;                  /// HUE change value
int16_t globalHueFrameDelay = 20;           /// Delay for hue changes in frames

/** overall LED brightness */
int16_t ledBrightness = LED_BRIGHTNESS;

int16_t currentFrameRate;                   /// Framerate of the current running effect

/**
 * @brief some ValueWrappers for global parameters
 * 
 */
ValueWrapper blendFactorW(&blendFactor, 0, 255, "Blend factor","BF", "System");
ValueWrapper ledBrightnessW(&ledBrightness, 1, 255, "Brightness", "LB", "System");
ValueWrapper currentFrameRateW(&currentFrameRate, 1, 5000, "Frame Rate","FR", "System");

ValueWrapper hueStepW(&globalHueStep, 0, 50, "HUE Step","HS","Palettes");
ValueWrapper hueFrameDelayW(&globalHueFrameDelay, 1, 500, "HUE Delay", "HFD", "Palettes");



/** the LED Buffers and the Matrix mapper */
CRGB leds[NUM_LEDS + 1];
CRGB backBuffer[NUM_LEDS + 1];

XYMatrix ledMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, leds,
#if USE_BACKBUFFER
  backBuffer,
#else
  NULL,
#endif
  SERPENTIME, FLIPPED, ROTATED);

void initialiseLEDs() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(1)>(leds, NUM_LEDS).setCorrection(COLOR_CORRECTION);
  FastLED.clear(true);
  FastLED.show();
  FastLED.setBrightness(LED_BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::DarkOrange);
  FastLED.show();
}

#if USE_BACKBUFFER
int backbufferBlender(unsigned long now, void *data) {
  //for (;;){
     //unsigned long now = millis();
     uint8_t frac = blendFactor;
     static uint8_t lastFrac = 0;
     if (frac != lastFrac) {
       //		Serial << "frac"<<frac<<endl;
       lastFrac = frac;
    }

#if DEBUG_EFFECTS
  Serial << ".";
#endif
  // if(frac < 4){
  // 	#if DEBUG_EFFECTS
  // 	Serial << "Frac cliped to 4, was "<<frac<<endl;
  // 	#endif
  // 	frac = 4;
  // }
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    CRGB pixel = blend(leds[i], backBuffer[i], frac);
    leds[i] = pixel;
  }
  // if( millis() < 5000 ) {
  // 	FastLED.setBrightness( scale8( Brightness.currentValue(), (millis() *
  // 256) / 5000)); } else { FastLED.setBrightness(Brightness.currentValue());
  // }
  FastLED.show();
  //vTaskDelay((1000 / 120) / portTICK_PERIOD_MS);
   return 0;
  //}
}
#endif

//
// The global palettes
//
PaletteList initializeSystemPalettes() {
  PaletteList tmp;
  tmp.push_back(new PalettePair("Rainbow",          Palette_t(RainbowColors_p, true)));
  tmp.push_back(new PalettePair("Rainbow Stripes",  Palette_t(RainbowStripeColors_p, false)));
  tmp.push_back(new PalettePair("Clouds",           Palette_t(CloudColors_p, false)));
  tmp.push_back(new PalettePair("Ocean",            Palette_t(OceanColors_p, false)));
  tmp.push_back(new PalettePair("Forest",           Palette_t(ForestColors_p, false)));
  tmp.push_back(new PalettePair("Party",            Palette_t(PartyColors_p, true)));
  tmp.push_back(new PalettePair("Lava",             Palette_t(LavaColors_p, false)));
  tmp.push_back(new PalettePair("Heat",             Palette_t(HeatColors_p, true)));
  tmp.push_back(new PalettePair("Arctic",           Palette_t(arctic_gp, true)));
  tmp.push_back(new PalettePair("Temperature",      Palette_t(temperature_gp, false)));
  tmp.push_back(new PalettePair("Colombia",         Palette_t(colombia_gp, false)));
  tmp.push_back(new PalettePair("Cequal",           Palette_t(cequal_gp, true)));
  tmp.push_back(new PalettePair("Sunset",           Palette_t(Another_Sunset_gp, false)));
  tmp.push_back(new PalettePair("Yellow Sunset",    Palette_t(Sunset_Yellow_gp, false)));
  return tmp;
}
PaletteList systemPalettes = initializeSystemPalettes();
PaletteList::iterator currentSystemPalette = systemPalettes.begin();

//
// The Effects 
//
EffectColorBands colorBandsEffect = EffectColorBands();
EffectTorch torchEffect = EffectTorch();
EffectNoise noiseEffect=EffectNoise();
EffectLava lavaEffect=EffectLava();
EffectRain rainEffect=EffectRain();
EffectTwinkle twinkleEffect = EffectTwinkle();
EffectPacifica pacificaEffect = EffectPacifica();
EffectWhitney whitneyEffect = EffectWhitney();

EffectList initializeSystemEffects() {
  EffectList tmp;

  tmp.push_back(&twinkleEffect);
  tmp.push_back(&noiseEffect);
  tmp.push_back(&torchEffect);
  tmp.push_back(&rainEffect);
  tmp.push_back(&colorBandsEffect);
  tmp.push_back(&pacificaEffect);
  tmp.push_back(&lavaEffect);
  tmp.push_back(&whitneyEffect);
  return tmp;
}

EffectList FastLEDAddOns::systemEffectList = initializeSystemEffects();
EffectList::iterator FastLEDAddOns::currentRunningEffect = systemEffectList.begin();

//
// A List of System Parameters. Only used for UI purposes
//
FastLEDAddOns::ParameterList initializeSystemParameterList(){
  FastLEDAddOns::ParameterList tmp;
  #if USE_BACKBUFFER
  tmp.push_back(&blendFactorW);
  #endif
  tmp.push_back(&hueStepW);
  tmp.push_back(&hueFrameDelayW);
  tmp.push_back(&ledBrightnessW);
  tmp.push_back(&currentFrameRateW);
  return tmp;
}
FastLEDAddOns::ParameterList systemParameterList = initializeSystemParameterList();


void startupBackbuffer(avrQueue *queue) {
#if USE_BACKBUFFER
    queue->scheduleFunction(backbufferBlender,NULL,"BBLD",0,1000/120);
  //xTaskCreate(backbufferBlender, "bbbl", 2048, NULL, 1, NULL);
  #endif
}

void startupEffects(avrQueue * queue,postFrame callback) {
  currentFrameRate = 1000/60;
  /*(void*)postFrameCallback*/

  queue->scheduleFunction(FastLEDAddOns::effectRunner,(void*)callback,"EFCT",0, 1000/60);  
}

void adjustFrameRate(avrQueue *queue, int16_t newFrameRate){
  Serial << "Change to " << (1000 / newFrameRate) << "(" << newFrameRate << ")"
         << endl;
  int result = queue->scheduleChangeFunction("EFCT", 0, (1000 / newFrameRate));
  Serial << "Result: " << result << endl;
}

ValueWrapper *wrapperForUIName(String uiName, FastLEDAddOns::ParameterList list){
  FastLEDAddOns::ParameterList::iterator piter = list.begin();
  while(piter != list.end()){
    if( (*piter)->getUIName() == uiName ){
      return (*piter);
    }
    piter++;
  }
  return NULL;
}

CHSV rgb2hsv(const CRGB& rgb)
{
	CHSV result = CHSV();
	double H,S,V;
	double min,max,delta;
	min = rgb.r<rgb.g?rgb.r:rgb.g;
	min = min < rgb.b?min:rgb.b;

	max = rgb.r>rgb.g?rgb.r:rgb.g;
	max = max  >rgb.b?max  :rgb.b;
	V = max;
	delta = max-min;
	if(max>0){
		S = (delta/max)*255;
	}else{
		S=0;
		H=0;
		V=0;
	}
	if(rgb.r>=max){
		H = (rgb.g - rgb.b)/delta;
	}else if(rgb.g>=max){
		H = 2.0+(rgb.b-rgb.r)/delta;
		}else
			H = 4.0+(rgb.r - rgb.g)/delta;

		H *=43.0;
//	Serial <<"H:"<<H<<" S:"<<S<<" V:"<<V<<endl;
		result.h = (uint8_t)H;
		result.s = (uint8_t)S;
		result.v = (uint8_t)V;
//		Serial <<"rH:"<<result.h<<" rS:"<<result.s<<" rV:"<<result.v<<endl;
		return result;
	}