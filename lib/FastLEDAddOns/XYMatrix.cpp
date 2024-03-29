/**
XYMatrix.cpp
**/

#include <XYMatrix.hpp>
#include <FastLED.h>
uint16_t XYMatrix::XY(FLPoint p){
  return XY(p.x, p.y);
}

uint16_t  XYMatrix::XY( uint8_t x, uint8_t y)
{
  uint16_t i = _width*_height;
  if( x<0 || x>(_width-1) || y<0 || y>(_height-1)){
    return i;
  }
  if(_isFlipped){
    y = (this->_height-1)-y;
  }

  if(_isRotated){
    if( this->_isSerpentine == false) {
      i = (x * this->_height) + y;
    }

    if( this->_isSerpentine == true) {
      if( x & 0x01) {
        // Odd rows run backwards
        uint8_t reverseY = (this->_height - 1) - y;
        i = (x * this->_height) + reverseY;
      } else {
        // Even rows run forwards
        i = (x * this->_height) + y;
      }
    }
  }else{
    if( this->_isSerpentine == false) {
      i = (y * this->_width) + x;
    }
    if( this->_isSerpentine == true) {
      if( y & 0x01) {
        // Odd rows run backwards
        uint8_t reverseX = (this->_width - 1) - x;
        i = (y * this->_width) + reverseX;
      } else {
        // Even rows run forwards
        i = (y * this->_width) + x;
      }
    }

  }
  return i;
}

void XYMatrix::setPixel(uint8_t x, uint8_t y, CRGB color){
  if( x<0 || x>(_width-1) || y<0 || y>(_height-1)){
    return;
  }
  uint16_t pixel = this->XY(x,y);
  if(!_backBuffer)
    _buffer[pixel] = color;
  else
    _backBuffer[pixel] = color;
}
void XYMatrix::setPixel(FLPoint p, CRGB color){
  this->setPixel(p.x, p.y, color);
}

CRGB XYMatrix::getPixel(uint8_t x, uint8_t y){
  CRGB result = CRGB::Black;
  if (x < 0 || x > (_width - 1) || y < 0 || y > (_height - 1)) {
    return result;
  }

  uint16_t pixel = this->XY(x,y);
  if(!_backBuffer)
    result =  _buffer[pixel] ;
  else
    result = _backBuffer[pixel];
  return result;
}

void XYMatrix::line(FLPoint st, FLPoint en, CRGB color) {
	int dx = abs(en.x-st.x), sx = st.x < en.x ? 1 : -1;
	int dy = -abs(en.y-st.y), sy = st.y < en.y ? 1 : -1;
	//Serial << "Dx:"<<dx<<" Dy:"<<dy<<" StepsC "<<sqrt((dx+dy)*(dx+dy));
	int aSteps = 0;
	int err = dx + dy, e2;
	for(;;) {
		setPixel(st,color);
		++aSteps;
		if (st.x == en.x && st.y == en.y) break;
		e2 = 2 * err;
		if (e2 > dy) {
			err += dy;
			st.x += sx;
		}
		if (e2 < dx) {
			err += dx;
			st.y += sy;
		}
	}
	//Serial << "StepsA "<<aSteps<<endl;
}

void XYMatrix::circle(_FLPoint p,int16_t radius, CRGB color,bool fill){
  int a = radius, b = 0;
  int radiusError = 1 - radius;
  CRGB* buf;

  if(!_backBuffer)
    buf = _buffer;
  else
    buf = _backBuffer;

  if (radius <= 0) {
    buf[XY(p)] = color;
    return;
  }

  while (a >= b)
  {
    if(fill){
      line(FLPoint(a+p.x,b+p.y),FLPoint(-a + p.x, b + p.y),color);
      line(FLPoint(b + p.x, a + p.y), FLPoint(-b + p.x, a + p.y), color);
      line(FLPoint(-a + p.x, -b + p.y),FLPoint(a + p.x, -b + p.y), color);
      line(FLPoint(-b + p.x, -a + p.y), FLPoint(b + p.x, -a + p.y), color);
    }else{
      buf[XY(a + p.x, b + p.y)] = color;
      buf[XY(-a + p.x, b + p.y)] = color;
      buf[XY(b + p.x, a + p.y)] = color;
      buf[XY(-b + p.x, a + p.y)] = color;
      buf[XY(-a + p.x, -b + p.y)] = color;
      buf[XY(a + p.x, -b + p.y)] = color;
      buf[XY(-b + p.x, -a + p.y)] = color;
      buf[XY(b + p.x, -a + p.y)] = color;
    }
    b++;
    if (radiusError < 0)
    radiusError += 2 * b + 1;
    else
    {
      a--;
      radiusError += 2 * (b - a + 1);
    }
  }
}

void XYMatrix::clearAll(){
  this->clear();
  for(uint16_t i=0;i<(_width*_height);i++){
		_buffer[i]=CRGB::Black;
	}
}
void XYMatrix::clear(){
  fill(CRGB::Black);
}

void XYMatrix::fill(CRGB color){
  CRGB* p = (_backBuffer)?_backBuffer:_buffer;
  for(uint16_t i=0;i<(_width*_height);i++){
    p[i]=color;
  }
}
void XYMatrix::fadeToBlack(uint8_t by){
  CRGB* p = (_backBuffer)?_backBuffer:_buffer;
  fadeToBlackBy(p,(_width*_height),by);
}

void XYMatrix::flush(){
  if(!_backBuffer){
    FastLED.show();
  }
}
