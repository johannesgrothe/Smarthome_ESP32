#pragma once

#include <algorithm>

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

    int i = int(h / 60.0);
    double f = (h / 60.0) - i;
    double p = val * (1 - sat);
    double q = val * (1 - f * sat);
    double t = val * (1 - (1 - f) * sat);

    if (sat == 0.0){
      r=g=b=v;
    }
    if (h <= 60 && h >= 0) {
      r = val, g = t, b = p;
    } else if (h > 60 && h <= 120 ) {
      r = q, g = val, b = p;
    } else if (h > 120 && h <= 180) {
      r = p, g = val, b = t;
    } else if (h > 180 && h <= 240) {
      r = p, g = q, b = val;
    } else if (h > 240 && h <= 300) {
      r = t, g = p, b = val;
    } else if (h > 300 && h <= 360) {
      r = val, g = p, b = q;
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
