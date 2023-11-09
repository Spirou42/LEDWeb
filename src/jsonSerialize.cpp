/**
 * @file jsonSerialize.cpp
 * @author Carsten Müller (carstenm@mac.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <jsonSerialize.h>
#include <Effect.hpp>
#include <EffectBase.h>

void jsonFromEffectList(JsonDocument* doc,  EffectList *list, EffectList::iterator *currentEffect)
{
  JsonArray effects = doc->createNestedArray("SystemEffects");
  EffectList::iterator liter = list->begin();
  uint16_t sequence = 0;
  while (liter != list->end()) {
    JsonObject obj = effects.createNestedObject();
    obj["effectName"] = (*liter)->name();
    obj["effectID"] = sequence;
    uint16_t k = ((*currentEffect) - (list->begin()));
    obj["current"] = sequence == k;
    //obj[(*liter)->name()] = sequence;
    liter++;
    sequence++;
  }
}

void jsonFromPaletteList(JsonDocument * doc, PaletteList *list, PaletteList::iterator *currentPalette)
{
  JsonArray palettes = doc->createNestedArray("SystemPalettes");
  PaletteList::iterator liter = list->begin();
  uint16_t sequence = 0;
  while (liter != list->end()) {
    JsonObject obj = palettes.createNestedObject();
    obj["PaletteName"] = (*liter)->first;
    obj["PaletteID"] = sequence;
    int k = ((*currentPalette) - (list->begin()));
    //Serial << "Types: " << k << endl;
    obj["current"] = sequence == k;
    //    obj[(*liter)->first.c_str()] = sequence;
    // palettes.add(obj);
    liter++;
    sequence++;
  }
}

void jsonFromValueWrapper(JsonObject* doc, ValueWrapper* wrapper)
{
  (*doc)["name"]      = wrapper->getName();
  (*doc)["value"]     = wrapper->getValue();
  (*doc)["maxValue"]  = wrapper->getMaxValue();
  (*doc)["minValue"]  = wrapper->getMinValue();
  (*doc)["uiName"]    = wrapper->getUIName();
  (*doc)["Hint"]      = wrapper->getCategory();
}

void jsonFromSystemParameters(JsonDocument* doc)
{
  JsonArray parameters = doc->createNestedArray("SystemParameters");
  FastLEDAddOns::ParameterList::iterator piter = systemParameterList.begin();
  uint16_t sequence = 0;
  while (piter != systemParameterList.end()) {
    JsonObject obj = parameters.createNestedObject();
    obj["ID"] = sequence;
    jsonFromValueWrapper(&obj, (*piter));
    piter++;
    sequence++;
  }
}

void jsonParametersFromEffect(JsonDocument* doc, EffectList::iterator *currentEffect)
{
/*  JsonObject object = doc->createNestedObject();*/
  Effect* effect = *(*currentEffect);
  (*doc)["effectName"] = effect->name();
  JsonArray parameterArray = doc->createNestedArray("Parameters");
  for (size_t i = 0; i < effect->numberOfParameters(); i++){
    JsonObject obj = parameterArray.createNestedObject();
    ValueWrapper *wrapper = effect->parameterAt(i);
    jsonFromValueWrapper(&obj, wrapper);
    obj["idx"] = i;
  }
}