/**
 * @file jsonSerialize.h
 * @author Carsten Müller (carstenm@mac.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include <ArduinoJson.h>
#include <Types.hpp>

void jsonFromPaletteList(JsonDocument *doc, PaletteList* list, PaletteList::iterator *currentPalette);
void jsonFromEffectList(JsonDocument *doc, EffectList* list, EffectList::iterator *currentEffect);
void jsonFromSystemParameters(JsonDocument *doc);
void jsonParametersFromEffect(JsonDocument *doc, EffectList::iterator *currentEffect);
