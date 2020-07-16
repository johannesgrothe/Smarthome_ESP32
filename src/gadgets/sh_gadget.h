#ifndef __SH_Gadget__
#define __SH_Gadget__

#include <cstring>
#include <functional>
#include <Arduino.h>
#include "ArduinoJson.h"
#include "../helping_structures.h"
#include "../system_settings.h"
#include "../console_logger.h"
#include "../connectors/connectors.h"

enum Gadget_Type {
  None, Lightbulb, Fan, Doorbell
};

enum SH_RGB_Color {
  SH_CLR_red, SH_CLR_green, SH_CLR_blue
};

enum SH_HSL_Color {
  SH_CLR_hue, SH_CLR_saturation, SH_CLR_lightness
};

class SH_Gadget : public IR_Connector, public Radio_Connector {
private:

  std::function<void(const char *, const char *, const char *, int)> updateRemotes;

  bool remoteInitialized;

  char name[GADGET_NAME_LEN_MAX]{};

  byte mapping_count{};

  Mapping_Reference *mapping[MAPPING_MAX_COMMANDS]{};

protected:

  bool initialized;

  bool has_changed;

  Gadget_Type type;

  void updateCharacteristic(const char *, int);

  const char *findMethodForCode(unsigned long);

  virtual void handleMethodUpdate(const char *);

public:
  SH_Gadget();

  explicit SH_Gadget(JsonObject, Gadget_Type);

  void initRemoteUpdate(std::function<void(const char *, const char *, const char *, int)> update_method);

  Gadget_Type getType();

  const char *getName();

  virtual bool getCharacteristics(char *characteristic_str) = 0;

  bool isInitialized();

  void handleCodeUpdate(unsigned long);

  virtual void handleCharacteristicUpdate(const char *characteristic, int value) = 0;

  virtual void refresh() = 0;

  virtual void print() = 0;

  void printMapping();

};

#endif //__SH_GAGDET__