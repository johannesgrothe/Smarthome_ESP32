#pragma once

//region DynamicJson
class DynamicJsonDocument {
private:
  uint16_t capacity_;
public:
  DynamicJsonDocument(uint16_t capacity);

  template<class T>
  T &operator[](std::string s) {
    return T();
  }
};

DynamicJsonDocument::DynamicJsonDocument(uint16_t capacity) :
  capacity_(capacity)
  {}

//T& DynamicJsonDocument::operator[](std::string s) {
//  return T();
//}
//endregion DynamicJson

//region JSONArray
class JsonArray {
private:
public:
};
//endregion JSONArray
