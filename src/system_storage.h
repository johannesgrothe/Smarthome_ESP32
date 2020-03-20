#ifndef __System_Storage__
#define __System_Storage__

#include "ArduinoJson.h"
#include <EEPROM.h>
#include "system_settings.h"

class System_Storage {
private:

public:

  System_Storage() {
  logger.println("Starting EEPROM...");
    int addr = 0;

// the sample text which we are storing in EEPROM
    char ssid[64] = "CARNIVAL OF RUST";

    if (!EEPROM.begin(EEPROM_SIZE)) {
      Serial.println("failed to init EEPROM");
      delay(1000000);
    }

    // writing byte-by-byte to EEPROM
    for (int i = 0; i < EEPROM_SIZE; i++) {
      EEPROM.write(addr, ssid[i]);
      addr += 1;
    }
    EEPROM.commit();

    // reading byte-by-byte from EEPROM
    for (int i = 0; i < EEPROM_SIZE; i++) {
      byte readValue = EEPROM.read(i);

      if (readValue == 0) {
        break;
      }

      char readValueChar = char(readValue);
      Serial.print(readValueChar);
    }
  };

  bool readConfig(DynamicJsonDocument *json_file) {
    logger.println(LOG_INFO, "Loading Config...");
    logger.incIndent();
    try {
      deserializeJson(*json_file, json_str);
    }
    catch (DeserializationError &e) {
      logger.println(LOG_ERR, "Cannot read JSON, creating blank Config.");
      deserializeJson(*json_file, default_config);
      logger.decIndent();
      return false;
    }
    logger.decIndent();
    return true;
  };

  bool writeConfig(JsonObject config_data) {
    logger.println(LOG_INFO, "Writing Config...");
    return false;
  };

};

#endif //__System_Storage__