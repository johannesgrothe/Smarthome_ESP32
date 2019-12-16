#include <string.h>
#include <Arduino.h>
#include "ArduinoJson.h"

#ifndef __SH_Gadget__
#define __SH_Gadget__

#define NAME_LEN 12

enum SH_Color {red, green, blue};

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
    uint8_t brightness;
    uint8_t default_brightness;
    uint8_t status;
    uint8_t hue[3];

  public:

    SH_Lamp(const char * name):
      SH_Receiver(name),
      brightness(100),
      default_brightness(100),
      status(1),
      hue({255, 255, 255})
      {
      };

    bool setBrightness(uint8_t new_brightness)
    {
      Serial.printf("Setting Status: %d\n", new_brightness);
      if (new_brightness > 0)
      {
        if (new_brightness > 100)
        {
          brightness = 100;
        }
        else
        {
          brightness = new_brightness;
        }
        status = true;
      }
      else
      {
        status = false;
        brightness = new_brightness;
      }
      return true;
    };

    uint8_t getBrightness()
    {
      return brightness;
    };

    bool setHue(uint8_t new_hue[3])
    {
      hue[0] = new_hue[0];
      hue[1] = new_hue[1];
      hue[2] = new_hue[2];
      return true;
    };

    bool setHue(uint8_t new_hue, uint8_t color_index)
    {
      hue[color_index] = new_hue;
    }

    uint8_t getHue(uint8_t color_index)
    {
      if (color_index > 2)
      {
        return false;
      }
      return hue[color_index];
    }

    bool toggleStatus()
    {
      setStatus(!status);
    };

    bool getStatus()
    {
      return status;
    };

    void setStatus(bool new_status)
    {
      Serial.printf("Setting Status: %d\n", new_status);
      if (new_status)
      {
        if (brightness < 15)
        {
          brightness = default_brightness;
        }
      }
      status = new_status;
    };

    bool decode(DynamicJsonDocument * doc)
    {
      Serial.println("Decoding...");
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
        setBrightness((uint8_t) obj["value"]);
      }
      Serial.println("Decoding Done.");
      return true;
    };

    bool getRegisterStr(char * buffer)
    {
      // sprintf(buffer, "{\"name\": \"%s\", \"service_name\": \"%s\", \"service\": \"Lightbulb\"}", name, name);
      sprintf(buffer, "{\"name\": \"%s\", \"service_name\": \"%s\", \"service\": \"Lightbulb\", \"Brightness\": \"default\", \"Hue\": \"default\", \"Saturation\": \"default\"}", name, name);
      // Serial.printf("RegStr: '%s'\n", buffer);
      return true;
    };
};

#endif