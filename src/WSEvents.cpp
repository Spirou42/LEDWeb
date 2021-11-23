/**
 * @file WSEvents.cpp
 * @author Carsten Müller (carstenm@mac.com)
 * @brief Creates and sends WS events
 * @version 0.1
 * @date 2021-10-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "WServer.h"
#include "WSEvents.h"
#include <LEDEffects.h>

void sendSystemPalettes(AsyncEventSource *events){
  String someString;
  DynamicJsonDocument document(2048);

  /* System Palettes */
  jsonFromPaletteList(&document, &systemPalettes, &currentSystemPalette);
  serializeJson(document, someString);
  events->send(someString.c_str(), "SPAL");
}

void sendSystemEffects(AsyncEventSource *events){
  String someString;
  DynamicJsonDocument document(2048);

  jsonFromEffectList(&document, &FastLEDAddOns::systemEffectList, &FastLEDAddOns::currentRunningEffect);
  serializeJson(document, someString);
  events->send(someString.c_str(), "SEFF");
}

void sendSystemParameters(AsyncEventSource *events){
  String someString;
  DynamicJsonDocument document(2048);

  jsonFromSystemParameters(&document);
  serializeJson(document, someString);
  events->send(someString.c_str(), "SPAR");
  document.clear();
}

void sendEffectParameters(AsyncEventSource *events) { 
  String someString;
  DynamicJsonDocument document(2048);
  jsonParametersFromEffect(&document, &FastLEDAddOns::currentRunningEffect);
  serializeJson(document, someString);
  events->send(someString.c_str(), "PAEF");
}