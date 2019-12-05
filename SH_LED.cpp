#include "SH_LED.h"

bool SH_LED::init(byte id, byte len, byte pin, byte dim_interval)
{
  led_stripe = Adafruit_NeoPixel(len, pin);
  led_stripe.begin();
  stripe_len = len;
  status = 0;
  brightness = DEFAULT_BRIGHTNESS;
  brightness_now = 0;
  brightness_start = 0;
  color[0] = 255;
  color[1] = 255;
  color[2] = 255;
  color_now[0] = 0;
  color_now[1] = 0;
  color_now[2] = 0;
  color_start[0] = 0;
  color_start[1] = 0;
  color_start[2] = 0;
  dimminterval = dim_interval;
  lastchange_clr = 0;
  lastchange_bri = 0;
  last_brightness = brightness;
  changing_clr = false;
  changing_bri = false;
  last_change_is_status = true;
  ident = id;
  isInitialized = true;
}

String SH_LED::getName()
{
  return String(ident);
}

byte SH_LED::getLength()
{
  return stripe_len;
}

String SH_LED::toString()
{
    return ("LED '" + String(ident) + "' ; Length: '" + stripe_len + "' ; Status: " + String(status) +
            "; Brightness: " +
            String(brightness) + "; Color: {" +
            String(color[0]) + ", " + String(color[1]) + ", " + String(color[2]) + "}");
}

byte SH_LED::getStatus()
{
    if (brightness == 0)
    {
        return 0;
    }
    return 1;
}

void SH_LED::setStatus(byte in) {
    if (in == 0) {
        last_brightness = brightness;
        brightness = 0;
    } else {
        if (last_brightness == 0) {
            brightness = DEFAULT_BRIGHTNESS;
        } else {
            brightness = last_brightness;
        }
    }
}

byte SH_LED::getBrightness() {
    return brightness;
}

void SH_LED::setBrightness(byte in) {
    if (in == 0) {
        last_brightness = brightness;
    }
    brightness = in;
}

String SH_LED::getColor() {
    return inttohex(color[0]) + inttohex(color[1]) + inttohex(color[2]);
    //return String(color[0]) + String(color[1]) + String(color[2]);
}

void SH_LED::setColor(String in) {
    last_change_is_status = false;
    color[0] = (byte) hextoint(colorpos(0, in));
    color[1] = (byte) hextoint(colorpos(1, in));
    color[2] = (byte) hextoint(colorpos(2, in));
    //Serial.println("New Color: {" + String(color[0]) + ", " + String(color[1]) + ", " + String(color[2]) + "}");
}

void SH_LED::setColor(byte index, byte in) {
    last_change_is_status = false;
    color[index] = in;
}

byte SH_LED::getColor(byte index) {
    return color[index];
}

byte SH_LED::getColor_now(byte index) {
    return color_now[index];
}

void SH_LED::setColor_now(byte index, byte in) {
    color_now[index] = in;
}

/*void SH_LED::refresh() {
    for (byte x = 0; x < stripe_len; x++) {
        led_stripe.setPixelColor(x, Adafruit_NeoPixel::Color(color[0] * (brightness / 100.0),
                                                             color[1] * (brightness / 100.0),
                                                             color[2] * (brightness / 100.0)));
    }
    led_stripe.show();
}*/


void SH_LED::refresh()
{
  if (!isInitialized) return;
  unsigned long now = millis();
  bool change_required = false;
  if (!isequal(color_now, color))
  {
    change_required = true;
    if (!changing_clr) {
      makeequal(color_start, color_now);
      changing_clr = true;
      lastchange_clr = 0;
      Serial.println("Starte Farbuebergang");
    }
    if (now > (lastchange_clr + (dimminterval / 100 * LED_STEP)))
    {
      lastchange_clr = now;
      for (byte i = 0; i < 3; i++)
      {
        int buff_factor = (int) (((color[i] - color_start[i]) / 100.0 * LED_STEP));
        if (buff_factor == 0 && (color_start[i] != color[i]))
        {
          if (color_start[i] < color[i])
          {
            buff_factor = 1;
          }
          else
          {
            buff_factor = -1;
          }
        }
        int buffer = color_now[i] + buff_factor;

        if ((color_start[i] < color[i] && buffer > color[i]) ||
          (color_start[i] > color[i] && buffer < color[i]))
        {
          buffer = color[i];
        }
        //Serial.println("Color[" + String(i) + "]: " + String(color_now[i]) + " -> " + String(buffer) + ";  Target: " + String(color[i]));
        color_now[i] = buffer;
      }
    }
  }
  else
  {
    if (changing_clr)
    {
      Serial.println("Farbuebergang beendet");
      changing_clr = false;
    }
  }

  if (brightness != brightness_now)
  {
    change_required = true;
    if (!changing_bri)
    {
      brightness_start = brightness_now;
      changing_bri = true;
      lastchange_bri = 0;
      Serial.println("Starte Helligkeitsuebergang");
    }
    if (now > (lastchange_bri + (dimminterval / 100 * LED_STEP)))
    {
      //Serial.println("Changing Brightness...");
      lastchange_bri = now;
      int buff_factor = (int) ((brightness - brightness_start) / 100.0 * LED_STEP);
      if (buff_factor == 0 != (brightness_start != brightness))
      {
        if (brightness_start < brightness)
        {
          buff_factor = 1;
        }
        else
        {
          buff_factor = -1;
        }
      }
      byte buffer = brightness_now + buff_factor;
      if ((brightness_start < brightness && buffer > brightness) || (brightness_start > brightness && buffer < brightness))
      {
        buffer = brightness;
      }
      //Serial.println("Brightness: " + String(brightness_now) + " -> " + String(buffer) + ";  Target: " + String(brightness));
      brightness_now = buffer;
    }
  }
  else
  {
    if (changing_bri)
    {
      Serial.println("Helligkeitsuebergang beendet, Helligkeit: " + String(brightness_now));
      changing_bri = false;
    }
  }

  if (change_required)
  {
    //Serial.println("Applying Changes...");
    for (byte x = 0; x < stripe_len; x++)
    {
      led_stripe.setPixelColor(x, Adafruit_NeoPixel::Color(color_now[0] * (brightness_now / 100.0),
                                                            color_now[1] * (brightness_now / 100.0),
                                                            color_now[2] * (brightness_now / 100.0)));
    }
    led_stripe.show();
  }
}

String SH_LED::colorpos(int i, String input)
{
  String out;
  out += input.charAt(i * 2);
  out += input.charAt((i * 2) + 1);
  return out;
}

int SH_LED::hextoint(String input) {
    int x = 0;
    for (int v = 0; v < input.length(); v++) {
        char c = input.charAt(v);
        if (c >= '0' && c <= '9') {
            x *= 16;
            x += c - '0';
        } else if (c >= 'A' && c <= 'F') {
            x *= 16;
            x += ((c - 'A') + 10);
        } else break;
    }
    // Serial.println(input + " - len: " + String(input.length()) + " - dec: " + String(x));
    return x;
}

void SH_LED::light_switch() {
    setStatus(!getStatus());
}

String SH_LED::digit_conv(int input) {
    switch (input) {
        case 10:
            return "A";
        case 11:
            return "B";
        case 12:
            return "C";
        case 13:
            return "D";
        case 14:
            return "E";
        case 15:
            return "F";
        default:
            return String(input);
    }
}

String SH_LED::inttohex(int input) {
    String outstr = "";
    while (input != 0) {
        outstr = digit_conv((input % 16)) + outstr;
        input = input / 16;
    }
    if (outstr.length() == 1) {
        outstr = '0' + outstr;
    } else if (outstr.length() == 0) {
        outstr = "00";
    }
    return outstr;
}

bool SH_LED::isequal(const byte first[3], const byte second[3]) {
    for (byte i = 0; i < 3; i++) {
        if (first[i] != second[i]) {
            return false;
        }
    }
    return true;
}

void SH_LED::makeequal(byte first[3], const byte second[3]) {
    for (byte i = 0; i < 3; i++) {
        first[i] = second[i];
    }
}

void SH_LED::testtest() {
    byte test1[] = {1, 2, 3};
    byte test2[] = {1, 2, 3};
    byte test3[] = {1, 3, 3};
    Serial.println(isequal(test1, test2));
    Serial.println(isequal(test1, test3));
    Serial.println(hextoint("FFFFFF"));
    Serial.println(hextoint("CCCCCC"));
    Serial.println(hextoint("F"));
    Serial.println(hextoint("FF"));
    Serial.println(hextoint("FFF"));
    Serial.println(hextoint("FFFF"));
    Serial.println(hextoint("FFFFF"));
    Serial.println(colorpos(0, "AAFFCC"));
    Serial.println(colorpos(1, "AAFFCC"));
    Serial.println(colorpos(2, "AAFFCC"));
}