#include "color.h"

// RGBColor
byte RGBColor::getRed() const {
  return red;
}

byte RGBColor::getGreen() const {
  return green;
}

byte RGBColor::getBlue() const {
  return blue;
}

void RGBColor::setColor(byte new_red, byte new_green, byte new_blue) {
  red = new_red;
  green = new_green;
  blue = new_blue;
}

// HSLColor
unsigned int HSLColor::getHue() const {
  return hue;
}

byte HSLColor::getSaturation() const {
  return saturation;
}

byte HSLColor::getLightness() const {
  return lightness;
}

void HSLColor::setColor(unsigned int new_hue, byte new_saturation, byte new_lightness) {
  hue = new_hue;
  saturation = new_saturation;
  lightness = new_lightness;
}


// HSVColor
unsigned int HSVColor::getHue() const {
  return hue;
}

byte HSVColor::getSaturation() const {
  return saturation;
}

byte HSVColor::getValue() const {
  return value;
}

void HSVColor::setColor(unsigned int new_hue, byte new_saturation, byte new_value) {
  hue = new_hue;
  saturation = new_saturation;
  value = new_value;
}

// Color
Color::Color() :
  rgb_color(),
  hsl_color(),
  hsv_color() {
  setRGB(0, 0, 0);
};

Color::Color(byte r, byte g, byte b) :
rgb_color(),
hsl_color(),
hsv_color() {
  setRGB(r, g, b);
};

RGBColor *Color::getRGB() {
  return &rgb_color;
}

HSLColor *Color::getHSL() {
  return &hsl_color;
}

HSVColor *Color::getHSV() {
  return &hsv_color;
}

byte Color::getBrightness() {
  return hsv_color.getValue();
}

unsigned int Color::getHue() {
  return hsv_color.getHue();
}

void Color::setRGB(byte red, byte green, byte blue) {
  double hsl[3];
  double hsv[3];
  rgb_color.setColor(red, green, blue);
  rgbToHsl(red, green, blue, hsl);
  rgbToHsv(red, green, blue, hsv);
  hsl_color.setColor((unsigned int) hsl[0], (byte) hsl[1], (byte) hsl[2]);
  hsv_color.setColor((unsigned int) hsv[0], (byte) hsv[1], (byte) hsv[2]);
}

void Color::setHSL(unsigned int hue, byte saturation, byte lightness) {
  double hsv[3];
  byte rgb[3];
  hsl_color.setColor(hue, saturation, lightness);
  hslToRgb(hue, saturation, lightness, rgb);
  rgb_color.setColor((byte) rgb[0], (byte) rgb[1], (byte) rgb[2]);
  rgbToHsv(rgb[0], rgb[1], rgb[2], hsv);
  hsv_color.setColor((unsigned int) hsv[0], (byte) hsv[1], (byte) hsv[2]);
}

void Color::setHSV(unsigned int hue, byte saturation, byte value) {
  double hsl[3];
  byte rgb[3];
  hsv_color.setColor(hue, saturation, value);
  hsvToRgb(hue, saturation, value, rgb);
  rgbToHsl(rgb[0], rgb[1], rgb[2], hsl);
  rgb_color.setColor((byte) rgb[0], (byte) rgb[1], (byte) rgb[2]);
  hsl_color.setColor((unsigned int) hsl[0], (byte) hsl[1], (byte) hsl[2]);
}

void Color::setBrightness(byte brightness) {
  setHSV(hsv_color.getHue(), hsv_color.getSaturation(), brightness);
}

void Color::setHue(unsigned int hue) {
  setHSV(hue, hsv_color.getSaturation(), hsv_color.getValue());
}