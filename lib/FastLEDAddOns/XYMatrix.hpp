/**
  XYMatrix.h

  mapping class for simple rectangular LED Mapping

  */

#pragma once
#include <Arduino.h>
#include <Streaming.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <Types.hpp>


class XYMatrix {
public:
  XYMatrix(uint8_t w, uint8_t h, CRGB* buffer,CRGB* bBuffer=NULL,bool serp=true,bool flipped=true,bool rot=true):_width(w),_height(h),_buffer(buffer),_backBuffer(bBuffer),_isSerpentine(serp),_isFlipped(flipped),_isRotated(rot){};

  uint8_t width(){return _width;}
  uint8_t height(){return _height;}

  void setPixel(uint8_t x, uint8_t y, CRGB color);
	void setPixel(FLPoint coord,CRGB color);
	CRGB getPixel(uint8_t x, uint8_t y);
	CRGB getPixel(FLPoint coord);
  void clear();

	void clearAll();
	void fill(CRGB color);
  void fadeToBlack(uint8_t by);

	void flush();
	void setBrightness(uint8_t b);
	uint8_t brightness();

  void setFlipped(bool flag){_isFlipped = flag;}
  bool isFlipped(){return _isFlipped;}
	// Bresenham line algorythm
	void line(FLPoint st, FLPoint en, CRGB color);
  void circle(FLPoint center, int16_t radius, CRGB color, bool fill = false);
  uint16_t XY(uint8_t x, uint8_t y);
  uint16_t XY(FLPoint p);

protected:
  uint8_t _width,_height;    ///< dimensions of the Matrix
  CRGB* _buffer;
  CRGB* _backBuffer;
  bool _isSerpentine;
  bool _isFlipped;
  bool _isRotated;
};


