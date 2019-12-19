#include <string.h>
#include <Arduino.h>
#include "ArduinoJson.h"
#include "colors.h"

#ifndef __SH_Gadget__
#define __SH_Gadget__

#define NAME_LEN 12

enum SH_RGB_Color {SH_CLR_red, SH_CLR_green, SH_CLR_blue};
enum SH_HSL_Color {SH_CLR_hue, SH_CLR_saturation, SH_CLR_lightness};

class SH_Gadget
{
  protected:
    const char * name;
    bool initialized;

  public:
    SH_Gadget():
      name(),
      initialized(false)
      {
      };

    SH_Gadget(const char * gadget_name):
      name(gadget_name),
      initialized(false)
      {
      };

    const char * getName()
    {
      return &name[0];
    }

    bool isInitialized()
    {
      return initialized;
    };

    virtual void refresh()
    {
    };

    virtual bool init()
    {
      initialized = true;
      return false;
    };

    virtual bool decode(DynamicJsonDocument * doc)
    {
      return false;
    }

    virtual bool getRegisterStr(char * buffer)
    {
      return false;
    }
};

class SH_Receiver : public SH_Gadget
{
  protected:
    bool status;

  public:

    SH_Receiver(const char * name):
      SH_Gadget(name)
      {
      };

};

class SH_Lamp : public SH_Receiver
{
  protected:
    float lightness;
    float last_lightness;
    float default_lightness;

    float saturation;

    float hue;

  public:

    SH_Lamp(const char * name):
      SH_Receiver(name),
      default_lightness(100),
      saturation(0),
      hue(0)
      {
      };

// Lightness
    bool setLightness(float new_brightness)
    {
      Serial.printf("[%s] Setting Brightness: %d\n", name, new_brightness);
      lightness = new_brightness;
      return false;
    };

    float getLightness()
    {
      return lightness;
    };

// Color (RGB)
    bool setColor(uint8_t r, uint8_t g, uint8_t b)
    {
      Serial.printf("[%s] Setting Color: [%d, %d, %d]\n", name, r, g, b);
      float hsl[3];
      rgbToHsl(r, g, b, &hsl[0]);
      hue = hsl[SH_CLR_hue];
      saturation = hsl[SH_CLR_saturation];
      lightness = hsl[SH_CLR_lightness];
      return false;
    };

    uint8_t getColor(uint8_t color_index)
    {
      float rgb[3];
      hslToRgb(hue, saturation, lightness, &rgb[0]);
      return rgb[color_index];
    }

    void getColor(float color_buffer[])
    {
      float rgb[3];
      hslToRgb(hue, saturation, lightness, &color_buffer[0]);
    }

// Hue
    bool setHue(float new_hue)
    {
      Serial.printf("[%s] Setting Hue: %.1f\n", name, new_hue);
      hue = new_hue;
      return true;
    }

    float getHue()
    {
      return hue;
    }

// Status
    bool toggleStatus()
    {
      setStatus(!getStatus());
    };

    bool getStatus()
    {
      if (lightness > 0) return true;
      return false;
    };

    void setStatus(bool new_status)
    {
      Serial.printf("[%s] Setting Status: %d\n", name, new_status);
      if (new_status == 0)
      {
        lightness = 0;
      }
      else
      {
        if (last_lightness > default_lightness)
        {
          lightness = last_lightness;
        }
        else
        {
          lightness = default_lightness;
        }
      }
    };

// Others
    bool decode(DynamicJsonDocument * doc)
    {
      JsonObject obj = doc->as<JsonObject>();
      if (obj["characteristic"] == "On")
      {
        setStatus((bool) obj["value"]);
      }
      else if (obj["characteristic"] == "Brightness")
      {
        setBrightness((uint8_t) obj["value"]);
      }
      else if (obj["characteristic"] == "Hue")
      {
        setHue((uint16_t) obj["value"]);
      }
      return true;
    };

    bool getRegisterStr(char * buffer)
    {
      // sprintf(buffer, "{\"name\": \"%s\", \"service_name\": \"%s\", \"service\": \"Lightbulb\"}", name, name);
      sprintf(buffer, "{\"name\": \"%s\", \"service_name\": \"%s\", \"service\": \"Lightbulb\", \"Brightness\": \"default\", \"Hue\": \"default\", \"Saturation\": \"default\"}", name, name);
      return true;
    };
};

#endif