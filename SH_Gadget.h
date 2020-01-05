#include <string.h>
#include <Arduino.h>
#include "ArduinoJson.h"
#include "colors.h"
#include "IR_Connector.h"

#ifndef __SH_Gadget__
#define __SH_Gadget__

enum SH_RGB_Color {SH_CLR_red, SH_CLR_green, SH_CLR_blue};
enum SH_HSL_Color {SH_CLR_hue, SH_CLR_saturation, SH_CLR_lightness};
enum SH_LAMP_TYPE {ON_OFF, BRI_ONLY, CLR_ONLY, CLR_BRI};

class SH_Gadget : public IR_Connector
{
  protected:
    const char * name;
    bool initialized;
    bool has_changed;

  public:
    SH_Gadget():
      name(),
      initialized(false),
      has_changed(true)
      {
      };

    SH_Gadget(const char * gadget_name):
      name(gadget_name),
      initialized(false),
      has_changed(true)
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

    virtual void refresh();

    virtual void print();
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
    float min_lightness;

    float saturation;

    float hue;

    SH_LAMP_TYPE type;

  public:

    SH_Lamp(const char * name, SH_LAMP_TYPE lamp_type):
      SH_Receiver(name),
      default_lightness(100.0),
      lightness(100.0),
      min_lightness(35),
      saturation(0),
      hue(0),
      type(lamp_type)
      {
      };

    // Lightness
    void setLightness(float new_lightness)
    {
      Serial.printf("[%s] Setting Lightness: %d\n", name, new_lightness);
      lightness = new_lightness;
      has_changed = true;
    };

    float getLightness()
    {
      return lightness;
    };

    // Color (RGB)
    void setColor(uint8_t r, uint8_t g, uint8_t b)
    {
      Serial.printf("[%s] Setting Color: [%d, %d, %d]\n", name, r, g, b);
      float hsl[3];
      rgbToHsl(r, g, b, &hsl[0]);
      hue = hsl[SH_CLR_hue];
      saturation = hsl[SH_CLR_saturation];
      lightness = hsl[SH_CLR_lightness];
      has_changed = true;
    };

    uint8_t getColor(uint8_t color_index)
    {
      uint8_t rgb[3];
      hslToRgb(hue, saturation, lightness, &rgb[0]);
      return rgb[color_index];
    }

    void getColor(uint8_t color_buffer[])
    {
      hslToRgb(hue, saturation, lightness, &color_buffer[0]);
    }

    // Hue
    void setHue(float new_hue)
    {
      Serial.printf("[%s] Setting Hue: %.1f\n", name, new_hue);
      hue = new_hue;
      has_changed = true;
    }

    float getHue()
    {
      return hue;
    }

    // Status
    void toggleStatus()
    {
      setStatus(!getStatus());
      has_changed = true;
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
        if (last_lightness > min_lightness)
        {
          lightness = last_lightness;
        }
        else
        {
          lightness = default_lightness;
        }
      }
      has_changed = true;
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
        setLightness((float) obj["value"]);
      }
      else if (obj["characteristic"] == "Hue")
      {
        setHue((float) obj["value"]);
      }
      return true;
    };

    bool getRegisterStr(char * buffer)
    {
      switch (type)
      {
        case ON_OFF :
          sprintf(buffer, "{\"name\": \"%s\", \"service_name\": \"%s\", \"service\": \"Lightbulb\"}", name, name);
          break;
        case BRI_ONLY :
          sprintf(buffer, "{\"name\": \"%s\", \"service_name\": \"%s\", \"service\": \"Lightbulb\", \"Brightness\": \"default\"}", name, name);
          break;
        case CLR_ONLY :
          sprintf(buffer, "{\"name\": \"%s\", \"service_name\": \"%s\", \"service\": \"Lightbulb\", \"Hue\": \"default\", \"Saturation\": \"default\"}", name, name);
          break;
        case CLR_BRI :
          sprintf(buffer, "{\"name\": \"%s\", \"service_name\": \"%s\", \"service\": \"Lightbulb\", \"Brightness\": \"default\", \"Hue\": \"default\", \"Saturation\": \"default\"}", name, name);
          break;
        default :
          return false;
      }
      return true;
    };

    void print()
    {
      Serial.println("LED:");
      Serial.printf("[%s] Hue: %.2f Lightness: %.2f Saturation: %.2f Status: %d\n", name, hue, lightness, saturation, getStatus());
    }
};

#endif