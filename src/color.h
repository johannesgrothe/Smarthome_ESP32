#ifndef __color__
#define __color__

#include "../.pio/libdeps/esp32cam/ArduinoJson_ID64/src/ArduinoJson/Namespace.hpp"


class RGBColor {
private:

  byte red;
  byte green;
  byte blue;

public:

  byte getRed() {
    return red;
  }

  byte getGreen() {
    return green;
  }

  byte getBlue() {
    return blue;
  }

  void setColor(byte new_red, byte new_green, byte new_blue) {
    red = new_red;
    green = new_green;
    blue = new_blue;
  }

};

class HSLColor {
private:

  unsigned int hue;
  byte saturation;
  byte lightness;

public:

  byte rgb[3];

  unsigned int getHue() {
    return hue;
  }

  byte getSaturation() {
    return saturation;
  }

  byte getLightness() {
    return lightness;
  }

  void setColor(unsigned int new_hue, byte new_saturation, byte new_lightness) {
    hue = new_hue;
    saturation = new_saturation;
    lightness = new_lightness;
  }

};

class HSVColor {
private:

protected:

public:
  unsigned int hue;
  byte saturation;
  byte value;

  unsigned int getHue() {
    return hue;
  }

  byte getSaturation() {
    return saturation;
  }

  byte getValue() {
    return value;
  }

  void setColor(unsigned int new_hue, byte new_saturation, byte new_value) {
    hue = new_hue;
    saturation = new_saturation;
    value = new_value;
  }

};

class Color {
private:
  RGBColor rgb_color;
  HSLColor hsl_color;
  HSVColor hsv_color;

  static void hsvToRgb(double h, double s, double v, uint8_t rgb[]) {
    double r, g, b;

    int i = int(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch (i % 6) {
      case 0:
        r = v, g = t, b = p;
        break;
      case 1:
        r = q, g = v, b = p;
        break;
      case 2:
        r = p, g = v, b = t;
        break;
      case 3:
        r = p, g = q, b = v;
        break;
      case 4:
        r = t, g = p, b = v;
        break;
      case 5:
        r = v, g = p, b = q;
        break;
    }

    rgb[0] = r * 255;
    rgb[1] = g * 255;
    rgb[2] = b * 255;
  }

  static double hue2rgb(double p, double q, double t) {
    if (t < 0) t += 1;
    if (t > 1) t -= 1;
    if (t < 1 / 6.0) return p + (q - p) * 6 * t;
    if (t < 1 / 2.0) return q;
    if (t < 2 / 3.0) return p + (q - p) * (2 / 3.0 - t) * 6;
    return p;
  }

  static void hslToRgb(unsigned int hue, byte saturation, byte lightness, byte rgb[]) {
    double h = hue / 360.0;
    double s = saturation / 100.0;
    double l = lightness / 100.0;

    double r, g, b;

    if (s == 0) {
      r = g = b = l; // achromatic
    } else {
      double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
      double p = 2 * l - q;
      r = hue2rgb(p, q, h + 1 / 3.0);
      g = hue2rgb(p, q, h);
      b = hue2rgb(p, q, h - 1 / 3.0);
    }

    rgb[0] = r * 255;
    rgb[1] = g * 255;
    rgb[2] = b * 255;
  }

  static double threeway_max(double a, double b, double c) {
    return max(a, max(b, c));
  }

  static double threeway_min(double a, double b, double c) {
    return min(a, min(b, c));
  }

  static void rgbToHsl(byte r, byte g, byte b, float hsl[]) {
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
      s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
      if (max == rd) {
        h = (gd - bd) / d + (gd < bd ? 6 : 0);
      } else if (max == gd) {
        h = ((bd - rd) + 2) / d;
      } else if (max == bd) {
        h = ((rd - gd) + 4) / d;
      }
      h /= 6;
    }
    hsl[0] = h * 360;
    hsl[1] = s * 100;
    hsl[2] = l * 100;
  }

  static void rgbToHsv(byte r, byte g, byte b, float hsv[]) {
    double rd = (double) r / 255;
    double gd = (double) g / 255;
    double bd = (double) b / 255;
    double max = threeway_max(rd, gd, bd), min = threeway_min(rd, gd, bd);
    double h, s, v = max;

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
  RGBColor *getRGB() {
    return &rgb_color;
  }

  HSLColor *getHSL() {
    return &hsl_color;
  }

  HSVColor *getHSV() {
    return &hsv_color;
  }

  void setRGB(byte red, byte green, byte blue) {
    float hsl[3];
    float hsv[3];
    rgb_color.setColor(red, green, blue);
    rgbToHsl(red, green, blue, hsl);
    rgbToHsv(red, green, blue, hsv);
    hsl_color.setColor(hsl[0], hsl[1], hsl[2]);
    hsv_color.setColor(hsv[0], hsv[1], hsv[2]);
  }

  void setHSL(unsigned int hue, byte saturation, byte lightness) {
    float hsv[3];
    byte rgb[3];
    hsl_color.setColor(hue, saturation, lightness);
    hslToRgb(hue, saturation, lightness, rgb);
    rgb_color.setColor(rgb[0], rgb[1], rgb[2]);
    rgbToHsv(rgb[0], rgb[1], rgb[2], hsv);
    hsv_color.setColor(hsv[0], hsv[1], hsv[2]);
  }

  void setHSV(unsigned int hue, byte saturation, byte value) {
    float hsl[3];
    byte rgb[3];
    hsv_color.setColor(hue, saturation, value);
    hsvToRgb(hue, saturation, value, rgb);
    rgb_color.setColor(rgb[0], rgb[1], rgb[2]);
    rgbToHsv(rgb[0], rgb[1], rgb[2], hsl);
    hsl_color.setColor(hsl[0], hsl[1], hsl[2]);
  }

};

#endif //__color__