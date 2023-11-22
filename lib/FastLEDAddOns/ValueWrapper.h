/**
 * @file ValueWrapper.h
 * @author Carsten Müller (carstenm@mac.com)
 * @brief declaration of the basic ValueWrapper
 * @version 0.1
 * @date 2023-11-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <Streaming.h>
#include <vector>

class ValueWrapperBase {
public:
  ValueWrapperBase(String tName = "", String tuiName = "",
                   String tcategory = "")
      : _name(tName), _uiName(tuiName), _category(tcategory) {}
  virtual ~ValueWrapperBase() {}

  virtual String &getName() { return _name; }
  virtual void setName(String &newName) { _name = newName; }

  virtual String &getCategory() { return _category; }
  virtual void setCategory(String &newCategory) { _category = newCategory; }

  virtual String &getUIName() { return _uiName; }
  virtual void setUiName(String &newUIName) { _uiName = newUIName; }

protected:
  String _name;
  String _uiName;
  String _category;
};

/**
 * @brief the ValueWrapper combines a single effect parameter with a name and a
 * min and max value
 * @todo change to template class
 */
//template <typename T>
typedef int16_t T;

class ValueWrapper : public ValueWrapperBase {
public:
  ValueWrapper() : ValueWrapperBase(), _value(NULL), _minVal(0), _maxVal(0){};
  ValueWrapper(T *val, T min, T max, String name,
               String uiName = "", String category = "")
      : ValueWrapperBase(name, uiName, category), _value(val), _minVal(min),
        _maxVal(max) {}
  virtual ~ValueWrapper() { _value = NULL; }
  virtual int16_t getValue() { return *_value; }
  virtual void setValue(T val) {
    if (val < _minVal) {
      val = _minVal;
    } else if (val > _maxVal) {
      val = _maxVal;
    }
    *_value = val;
  }

  virtual void setValueStore(T *valStore) { _value = valStore; }
  virtual T getMinValue() { return _minVal; }
  virtual void setMinValue(T newMin) { _minVal = newMin; }
  
  virtual T getMaxValue() { return _maxVal; }
  virtual void setMaxValue(T newMax) { _maxVal = newMax; }

 
  friend Print &operator<<(Print &obj, ValueWrapper *v) {
    if (v) {
      obj << "[" << v->_name << "]{" << *(v->_value) << ", " << v->_minVal
          << ", " << v->_maxVal << "}";
    } else {
      obj << "[NONE]";
    }
    return obj;
  }

protected:
  T *_value;
  T _minVal;
  T _maxVal;
};