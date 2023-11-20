/**
Effects.hpp
*/

#ifndef __EFFECT_H__
#define __EFFECT_H__
#include <Arduino.h>
#include <FastLEDAddOns_Intern.h>
#include <Types.hpp>
#include <ValueWrapper.h>

/**
baseclass for LED Effects
*/
ADDONS_NAMESPACE_BEGIN
typedef std::vector<ValueWrapper *> ParameterList;
ADDONS_NAMESPACE_END
class Effect {

public:
  Effect() : _name(String()), _lastPalette(systemPalettes.end()) {
    _lastPalette = systemPalettes.end();
  };
  Effect(String name) : _name(name), _lastPalette(systemPalettes.end()) {
    _lastPalette = systemPalettes.end();
  };

  /**
                called after the effect programm got switch and before the first
     frame. put your book keeping and initialization in here
        */
  virtual void startEffect() {
    if (_lastPalette != systemPalettes.end()) {
      currentSystemPalette = _lastPalette;
    }
    if (_parameters.empty()) {
      initializePublicParameter();
    }
  }; ///< book keeping on startup

  /**
                called for each and every frame
        */
  virtual void frame(unsigned long now) {
    if (_lastPalette != currentSystemPalette) {
      _lastPalette = currentSystemPalette;
    }
  };
  virtual void initializePublicParameter() {}
  /** the framerate for the effect */
  virtual uint16_t frameRate() { return 1000 / 60; }
  /**
  called after the last frame. restore your book keeping here and clean up your
  dynamic objects
  */
  virtual void stopEffect(){};

  String name() { return _name; }

  /** parameter handling
  user controlable parameters are stored in
  */
  FastLEDAddOns::ParameterList::iterator parameters() {
    return _parameters.begin();
  }

  virtual size_t numberOfParameters() {
    return _parameters.end() - _parameters.begin();
  };
  virtual ValueWrapper *parameterAt(size_t idx) {
    Serial << "Gnartz";
    return NULL;
  };
  virtual String parameterNameAt(size_t idx) {
    Serial << "Gnartz2";
    return "";
  };

protected:
  void addParameter(ValueWrapper *value) { _parameters.push_back(value); }
  FastLEDAddOns::ParameterList _parameters;
  String _name;
  PaletteList::iterator _lastPalette;
};

/** if you use the effectRunner method you have to declare the following: **/

ADDONS_NAMESPACE_BEGIN
extern EffectList systemEffectList;
extern EffectList::iterator currentRunningEffect;

int effectRunner(unsigned long now, void *userdata);
extern volatile int16_t requestedFrameRate;
ADDONS_NAMESPACE_END
#endif
