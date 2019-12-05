#ifndef __SH_LED__
#define __SH_LED__

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__

#include <avr/power.h>

#endif

#define LED_STEP 5
#define DEFAULT_BRIGHTNESS 100

class SH_LED {
private:
    Adafruit_NeoPixel led_stripe;
    byte stripe_len;
    byte ident;
    byte status;
    byte brightness;
    byte brightness_now;
    byte brightness_start;
    byte color[3];
    byte color_now[3];
    byte color_start[3];
    bool changing_clr;
    bool changing_bri;
    byte last_brightness;
    bool last_change_is_status;

    bool isInitialized;

    unsigned long lastchange_clr;
    unsigned long lastchange_bri;
    byte dimminterval = 400;


    static int hextoint(String input);

    static String inttohex(int input);

    static String digit_conv(int input);

    static String colorpos(int i, String input);

    static bool isequal(const byte first[3], const byte second[3]);

    static void makeequal(byte first[3], const byte second[3]);

public:

    SH_LED():
      color(),
      color_now(),
      color_start(),
      isInitialized(false)
      {
      };

    String toString();

    byte getLength();

    String getName();

    byte getStatus();

    void setStatus(byte in);

    byte getBrightness();

    void setBrightness(byte in);

    String getColor();

    void setColor(String in);

    byte getColor(byte index);

    void setColor(byte index, byte in);

    byte getColor_now(byte index);

    void setColor_now(byte index, byte in);

    void refresh();

    static void testtest();

    void light_switch();

    bool init(byte id, byte len, byte pin, byte dim_interval);
};

#endif
