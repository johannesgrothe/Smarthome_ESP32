#ifndef __color__
#define __color__

class Color {
private:
  RGBColor rgb_color;
  HSLColor hsl_color;
  HSVColor hsv_color;

public:
  RGBColor getRGB(){

  }

  HSLColor getHSL(){

  }

  HSVColor getHSV(){

  }

  void setRGB(byte red, byte green, byte blue){
    rgb_color.setColor(red, green, blue)
  }

  void setHSL(unsigned int hue, byte saturation, byte lightness){

  }

  void setHSV(unsigned int hue, byte saturation, byte value){

  }

};

class RGBColor {
private:

public:
  byte red;
  byte green;
  byte blue;

  byte getRed(){

  }

  byte getGreen(){

  }

  byte getBlue(){

  }

  void setColor(byte red, byte green, byte blue){

  }

};

class HSLColor {
private:

public:
  unsigned int hue;
  byte saturation;
  byte lightness;

  unsigned int getHue(){

  }

  byte getSaturation(){

  }

  byte getLightness(){

  }

  void setColor(unsigned int hue, byte saturation, byte lightness){

  }

};

class HSVColor {
private:

protected:

public:
  unsigned int hue;
  byte saturation;
  byte value;

  unsigned int getHue(){

  }

  byte getSaturation(){

  }

  byte getValue(){

  }

  void setColor(unsigned int hue, byte saturation, byte value){

  }

};

#endif //__color__