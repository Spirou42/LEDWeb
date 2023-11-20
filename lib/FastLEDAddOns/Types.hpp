/**
Types.h
a collection of usefull Types, mostly collections
*/
// some datatype to map Names(Strings) palettes or Effects

#pragma once
#include <Arduino.h>
#include <Streaming.h>

#include <vector>

// #include <tuple>
#define FASTLED_INTERNAL
#include <FastLED.h>
class Effect;

/** some usefull collections */

/** a list of named Palettes, easy to synchronise with a selection menu */
struct Palette_t {
public:
  Palette_t(CRGBPalette16 p, bool b = false) : palette(p), loop(b){};
  CRGBPalette16 palette;
  bool loop;
};

/** naming a palette */
typedef std::pair<const String, Palette_t> PalettePair;
/** creating a list of named paletts */
typedef std::vector<PalettePair *> PaletteList;

/** a list of named effect functions as a simple way to implement effects*/
typedef void (*simpleEffectHandler)(void);
/** naming the function */
typedef std::pair<const String, simpleEffectHandler> SimpleEffectPair;
/** creating a list of named functions */
typedef std::vector<SimpleEffectPair *> SimpleEffectList;

/** creating a list of more complex effects */
typedef std::vector<Effect *> EffectList;

/**
  you have to define and initialise the following by yourself
**/
extern PaletteList systemPalettes;
extern PaletteList::iterator currentSystemPalette;

/**
 * @brief Geometry Types
 *
 */
typedef struct _FLPoint {
  int16_t x;                                     ///< x-part
  int16_t y;                                     ///< y-part
  _FLPoint(int16_t X, int16_t Y) : x(X), y(Y){}; ///< Basic constructor
  _FLPoint() : x(0), y(0){}; ///< empty constructor creates a (0,0)
  _FLPoint(const _FLPoint &k) {
    x = k.x;
    y = k.y;
  };
  bool operator==(_FLPoint p) { return (x == p.x) && (y == p.y); }
  bool operator!=(_FLPoint p) { return !(*this == p); }
  _FLPoint operator+=(_FLPoint p) {
    x += p.x;
    y += p.y;
    return *this;
  };
  //_FLPoint operator+(_FLPoint &p){_FLPoint r; r.x=p.x+x;r.y=p.y+y;return r;}
  _FLPoint operator+(_FLPoint p) {
    _FLPoint r;
    r.x = p.x + x;
    r.y = p.y + y;
    return r;
  }
} FLPoint;

typedef struct _FLSize {
  int16_t w, h;
  _FLSize() : w(0), h(0){};
  _FLSize(const _FLSize &k) {
    w = k.w;
    h = k.h;
  };
  _FLSize(int16_t w, int16_t h) : w(w), h(h){};
  bool operator==(_FLSize s) { return (s.w == w) && (s.h == h); }
  bool operator!=(_FLSize s) { return !(*this == s); }
  _FLSize operator+=(_FLSize s) {
    w += s.w;
    h += s.h;
    return *this;
  };
  _FLSize operator+(_FLSize p) {
    _FLSize r;
    r.w = p.w + w;
    r.h = p.h + h;
    return r;
  }
  _FLPoint operator+(_FLPoint p) {
    _FLPoint r;
    r.x = p.x + w;
    r.y = p.y + h;
    return r;
  }
  _FLSize operator+=(int t) {
    w += t;
    h += t;
    return *this;
  };
  _FLSize operator*(int k) {
    _FLSize l;
    l.w = k * w;
    l.h = k * h;
    return l;
  }
} FLSize;

typedef struct _FLRect {
  FLPoint origin;
  FLSize size;
  _FLRect() : origin(), size(){};
  _FLRect(const _FLRect &k) {
    origin = k.origin;
    size = k.size;
  };
  _FLRect(FLPoint o, FLSize s) {
    origin = o;
    size = s;
  };
  _FLRect(int16_t x, int16_t y, int16_t w, int16_t h)
      : origin(x, y), size(w, h){};
} FLRect;

typedef enum _UIHint {

} UIHint;
