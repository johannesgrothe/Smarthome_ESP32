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
    char name[NAME_LEN + 1];
    bool initialized;

  public:
    SH_Gadget():
      name(),
      initialized(false)
      {
      };

    SH_Gadget(const char * name):
      name(),
      initialized(false)
      {
        setName(name);
      };

    void setName(const char * new_name)
    {
      strcpy(name, new_name);
      name[NAME_LEN] = '\0';
    };

    String getName()
    {
      return String(name);
    };

    bool isInitialized()
    {
      return initialized;
    };

    void refresh()
    {
    };

    bool init()
    {
      initialized = true;
    };

    bool decode(DynamicJsonDocument doc)
    {
      return false;
    }

    virtual bool getRegisterStr(char * buffer) = 0;

    // virtual bool getRegisterStr(char * buffer)
    // {
    //   Serial.println("PARENT METHOD");
    //   return false;
    // }
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
      if (new_status)
      {
        if (brightness < 15)
        {
          brightness = default_brightness;
        }
      }
      status = new_status;
    };

    bool decode(DynamicJsonDocument doc)
    {
      if (doc["characteristic"] == "On")
      {
        setStatus(doc["value"]);
      }
      else if (doc["characteristic"] == "Brightness")
      {
        setBrightness(doc["value"]);
      }
      else if (doc["characteristic"] == "Hue")
      {
        setBrightness(doc["value"]);
      }
    };

    bool getRegisterStr(char * buffer)
    {
      Serial.println("RegStr");
      sprintf(buffer, "{\"name\": \"%s\", \"service_name\": \"%s\", \"service\": \"Lightbulb\"}", name, name);
      Serial.println(buffer);
      Serial.println("RegStr End");
      return true;
    };
};

#endif