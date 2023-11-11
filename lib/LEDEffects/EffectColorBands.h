  /*
  EffectColorBands.h
  */
  
  
  #ifndef __EFECT_LINEBOUNCE_H__
  #define __EFECT_LINEBOUNCE_H__

  #include "EffectBase.h"

  class EffectColorBands : public Effect {
  public:
   EffectColorBands()
       : Effect("Color Bands"), _bandColorDiff(20), _bandWidth(8){};

   virtual void startEffect();
   virtual uint16_t frameRate();
   virtual void frame(unsigned long now);
   virtual ValueWrapper* parameterAt(size_t idx);
   virtual String parameterNameAt(size_t idx);

  protected:
   int16_t _bandColorDiff;
   int16_t _bandWidth;
   int16_t _verticalBands;      // eigendlich ein boolean, wird es auch wenn der valueWrapper ein Template wird
   int16_t _direction;
  };

#endif
