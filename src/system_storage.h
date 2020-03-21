#ifndef __System_Storage__
#define __System_Storage__

#include "ArduinoJson.h"
#include <EEPROM.h>
#include "system_settings.h"
#include "console_logger.h"

class System_Storage {
private:

public:

  System_Storage() {
    logger.println("Initializing EEPROM...");

    if (!EEPROM.begin(EEPROM_CONFIG_LEN_MAX)) {
      logger.println(LOG_ERR, "failed to initialize EEPROM");
    }
  };

  void testRead(int len) {
    for (int i = 0; i < len; i++) {
      char readValue = EEPROM.readChar(i);
      Serial.print(readValue);
    }
    Serial.println();
  }

  bool readConfig(char *buffer) {
    logger.println(LOG_INFO, "Loading Config...");
    logger.incIndent();

    char first_char = char(EEPROM.read(0));
    if (first_char == '{') {
      byte json_level = 1;
      buffer[0] = first_char;
      for (int i = 1; i < EEPROM_CONFIG_LEN_MAX; i++) {
        byte readValue = EEPROM.read(i);

        char readValueChar = char(readValue);
        buffer[i] = readValueChar;

        if (readValueChar == '}')
          json_level--;
        else if (readValueChar == '{')
          json_level++;

        if (json_level == 0 || readValue == 0) {
          break;
        }
      }
    } else {
      logger.println(LOG_ERR, "Couldn't load: corrupted data.");
//      writeConfig(buffer);
      testRead(50);
    }
    Serial.println(buffer);
    strncpy(buffer, &json_str[0], EEPROM_CONFIG_LEN_MAX);
    logger.decIndent();
    return true;
  };

  bool writeConfig(JsonObject json_config) {
    char buffer[EEPROM_CONFIG_LEN_MAX]{};

    DynamicJsonDocument buf = DynamicJsonDocument(json_config);
//      json_config.as<DynamicJsonDocument>();
    deserializeJson(buf, &buffer[0], 0);

    return writeConfig(buffer);
  }

  bool writeConfig(const char *config_str) {
    if (strlen(config_str) > EEPROM_CONFIG_LEN_MAX) {
      logger.println(LOG_ERR, "Cannot write config: too long");
      return false;
    }
    logger.println(LOG_INFO, "Writing Config...");
    bool inside_string = false;
    int write_index = 0;
    int chars_deleted = 0;
    int chars_written = 0;
    logger.incIndent();
    logger.print(LOG_DATA, "");
    for (int i = 0; i < strlen(config_str); i++) {
      char write_char = char(config_str[i]);

      if (write_char == '"')
        inside_string = !inside_string;

      if (write_char == '\n' || (write_char == ' ' && !inside_string)) {
        chars_deleted++;
      } else {
        chars_written++;
        logger.add(write_char);
        EEPROM.writeChar(write_index, default_config[i]);
        write_index++;
      }
    }
    logger.addln();
    EEPROM.commit();
    logger.print(LOG_DATA, "Bytes written: ");
    logger.addln(chars_written);
    logger.print(LOG_DATA, "Bytes deleted: ");
    logger.addln(chars_deleted);
    logger.decIndent();
    testRead(60);
    return true;
  };

};

#endif //__System_Storage__