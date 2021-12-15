/**
 * @file MatrixDemo.h
 * @author Carsten Müller (carstenm@mac.com)
 * @brief Simple Maxtrix Demonsatration effects
 * @version 0.1
 * @date 2021-09-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <FastLEDAddOns.h>
#include <avrQueue.h>

#define USE_BACKBUFFER 1
#define USE_CHRISTMASBALL 0
#define USE_CYLINDERLAMP 1

#define DEBUG_EFFECTS 0

#if USE_CHRISTMASBALL
  #define MATRIX_WIDTH      13
  #define MATRIX_HEIGHT     10
#elif USE_CYLINDERLAMP
  #define MATRIX_WIDTH      10
  #define MATRIX_HEIGHT     20
#endif

#define LED_TYPE            APA102
#define DATA_PIN            23
#define CLOCK_PIN           18
#define COLOR_ORDER         BGR
#define COLOR_CORRECTION    0xffffff
#define LED_BRIGHTNESS      128

#define NUM_LEDS          (MATRIX_WIDTH*MATRIX_HEIGHT)



extern XYMatrix ledMatrix;
extern PaletteList systemPalettes;
extern PaletteList::iterator currentSystemPalette;

extern EffectList systemEffectList;
extern EffectList::iterator currentRunningEffect;

extern FastLEDAddOns::ParameterList systemParameterList;

extern int16_t blendFactor;

extern uint8_t globalHue;
extern int16_t globalHueStep ;
extern int16_t globalHueFrameDelay ;

extern int16_t ledBrightness;
extern int16_t currentFrameRate;                   /// Framerate of the current running effect

typedef void (*postFrame)(unsigned long );

extern CRGB leds[];
extern CRGB backBuffer[];

#ifndef MATRIX_HEIGHT
#error You have to declare either USE_CHRISTMASBALL or USE_CYLINDERLAMP
#endif

#define PALETTE_RAINBOW 0
#define PALETTE_RAINBOW_STRIPES 1
#define PALETTE_CLOUDS 2
#define PALETTE_OCEAN 3
#define PALETTE_FOREST 4
#define PALETTE_PARTY 5
#define PALETTE_LAVA 6
#define PALETTE_HEAT 7
#define PALETTE_ARCTIC 8
#define PALETTE_TEMPERATURE 9
#define PALETTE_COLOMBIA 10
#define PALETTE_CEQUAL 11
#define PALETTE_SUNSET 12
#define PALETTE_YELLOW_SUNSET 13

void initialiseLEDs();
void startupEffects(avrQueue *, postFrame);
void startupBackbuffer(avrQueue *queue);
void adjustFrameRate(avrQueue *queue, int16_t newFrameRate);
ValueWrapper *wrapperForUIName(String uiName, FastLEDAddOns::ParameterList list);