#pragma once

#include <algorithm>
#include <cstdlib>
#include <cmath>
#include "console_logger.h"

using byte = unsigned char;

class RGBColor {
private:

  byte red{};
  byte green{};
  byte blue{};

public:

  byte getRed() const;

  byte getGreen() const;

  byte getBlue() const;

  void setColor(byte new_red, byte new_green, byte new_blue);

};

class HSLColor {
private:

  unsigned int hue{};
  byte saturation{};
  byte lightness{};

public:

  unsigned int getHue() const;

  byte getSaturation() const;

  byte getLightness() const;

  void setColor(unsigned int new_hue, byte new_saturation, byte new_lightness);

};

class HSVColor {
private:

protected:

public:
  unsigned int hue;
  byte saturation;
  byte value;

  unsigned int getHue() const;

  byte getSaturation() const;

  byte getValue() const;

  void setColor(unsigned int new_hue, byte new_saturation, byte new_value);

};

class Color {
private:
  RGBColor rgb_color;
  HSLColor hsl_color;
  HSVColor hsv_color;

  static void hsvToRgb(unsigned int h, byte s, byte v, byte rgb[]) {
    double r = 0, g = 0, b = 0;
    double sat = s / 100.0;
    double val = v / 100.0;
    double i = h / 60.0;
    double c = val * sat;
    double res = std::fmod(i,2) - 1;
    double x = c * (1 - abs(res));
    double m = val - c;

    if (sat == 0.0){
      r=g=b=v;
    }
    if (h >= 0 && h < 60) {
      r = c, g = x, b = 0;
    } else if (h >= 60 && h < 120 ) {
      r = x, g = c, b = 0;
    } else if (h >= 120 && h < 180) {
      r = 0, g = c, b = x;
    } else if (h >= 180 && h < 240) {
      r = 0, g = x, b = c;
    } else if (h >= 240 && h < 300) {
      r = x, g = 0, b = c;
    } else if (h >= 300 && h < 360) {
      r = c, g = 0, b = x;
    }

    rgb[0] = (r + m) * 255;
    rgb[1] = (g + m) * 255;
    rgb[2] = (b + m) * 255;
  }

  static void hslToRgb(unsigned int hue, byte saturation, byte lightness, byte rgb[]) {
    double r = 0, g = 0, b = 0;
    double s = saturation / 100.0;
    double l = lightness / 100.0;

    if (s < 0 || s > 1 || l < 0 || l > 1) {
      logger_e("HSL Color", "saturation or lightness were either set below 0% or above 100%, which is not whithin the color scheme");
      return;
    }

    double i = hue / 60.0;
    double c = (1 - abs(2*l - 1)) * s;
    double res = std::fmod(i,2) - 1;
    double x = c * (1 - abs(res));
    double m = l - (c/2);

    if (s == 0.0){
      r=g=b=l;
    }
    if (hue >= 0 && hue < 60) {
      r = c, g = x, b = 0;
    } else if (hue >= 60 && hue < 120 ) {
      r = x, g = c, b = 0;
    } else if (hue >= 120 && hue < 180) {
      r = 0, g = c, b = x;
    } else if (hue >= 180 && hue < 240) {
      r = 0, g = x, b = c;
    } else if (hue >= 240 && hue < 300) {
      r = x, g = 0, b = c;
    } else if (hue >= 300 && hue < 360) {
      r = c, g = 0, b = x;
    }
    rgb[0] = (r + m) * 255;
    rgb[1] = (g + m) * 255;
    rgb[2] = (b + m) * 255;
  }

  static double threeway_max(double a, double b, double c) {
    return std::max(a, std::max(b, c));
  }

  static double threeway_min(double a, double b, double c) {
    return std::min(a, std::min(b, c));
  }

  static void rgbToHsl(byte r, byte g, byte b, double hsl[]) {
    double rd = (double) r / 255;
    double gd = (double) g / 255;
    double bd = (double) b / 255;
    double max = threeway_max(rd, gd, bd);
    double min = threeway_min(rd, gd, bd);
    double h = 0, s = 0, l = (max + min) / 2;

    if (max == min) {
      h = s = 0; // achromatic
    } else {
      double d = max - min;
      s = d == 0 ? 0 : d / (1 -abs(2*l - 1));
      if (max == rd) {
        h = (gd - bd) / d + (gd < bd ? 6 : 0);
      } else if (max == gd) {
        h = (bd - rd) / d + 2;
      } else if (max == bd) {
        h = (rd - gd) / d + 4;
      }
      h /= 6;
    }
    hsl[0] = h * 360;
    hsl[1] = s * 100;
    hsl[2] = l * 100;
  }

  static void rgbToHsv(byte r, byte g, byte b, double hsv[]) {
    double rd = (double) r / 255.0;
    double gd = (double) g / 255.0;
    double bd = (double) b / 255.0;
    double max = threeway_max(rd, gd, bd), min = threeway_min(rd, gd, bd);
    double h = 0, s = 0, v = max;

    double d = max - min;
    s = max == 0 ? 0 : d / max;

    if (max == min) {
      h = 0; // achromatic
    } else {
      if (max == rd) {
        h = (gd - bd) / d + (gd < bd ? 6 : 0);
      } else if (max == gd) {
        h = (bd - rd) / d + 2;
      } else if (max == bd) {
        h = (rd - gd) / d + 4;
      }
      h /= 6;
    }

    hsv[0] = h * 360;
    hsv[1] = s * 100;
    hsv[2] = v * 100;
  }

public:
  Color();

  Color(byte r, byte g, byte b);

  RGBColor *getRGB();

  HSLColor *getHSL();

  HSVColor *getHSV();

  byte getBrightness();

  unsigned int getHue();

  void setRGB(byte red, byte green, byte blue);

  void setHSL(unsigned int hue, byte saturation, byte lightness);

  void setHSV(unsigned int hue, byte saturation, byte value);

  void setBrightness(byte brightness);

  void setHue(unsigned int hue);
};
