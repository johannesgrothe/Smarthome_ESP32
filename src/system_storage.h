#pragma once

#include "ArduinoJson.h"
#include <EEPROM.h>
#include "system_settings.h"
#include "console_logger.h"

static bool validateJson(const char *new_json_str) {
  DynamicJsonDocument json_file(2048);
  DeserializationError err = deserializeJson(json_file, new_json_str);
  return err == DeserializationError::Ok;
}

class System_Storage {
private:

  static bool writeConfigStr(const char *config_str) {
    logger.println(LOG_TYPE::INFO, "Writing...");
    bool inside_string = false;
    int write_index = 0;
    int chars_deleted = 0;
    int chars_written = 0;
    logger.incIndent();
    logger.print(LOG_TYPE::DATA, "");

    char write_buffer[EEPROM_CONFIG_LEN_MAX]{};
    for (int i = 0; i < strlen(config_str); i++) {
      char write_char = char(config_str[i]);

      if (write_char == '"')
        inside_string = !inside_string;

      if (write_char == '\n' || (write_char == ' ' && !inside_string)) {
        chars_deleted++;
      } else {
        chars_written++;
        logger.add(write_char);
        EEPROM.writeChar(write_index, config_str[i]);
        write_buffer[write_index] = config_str[i];
        write_index++;
      }
    }
    EEPROM.commit();

    logger.addln();
    logger.print(LOG_TYPE::DATA, "Bytes written: ");
    logger.addln(chars_written);
    logger.print(LOG_TYPE::DATA, "Bytes deleted: ");
    logger.addln(chars_deleted);
    logger.decIndent();
    bool everything_ok = true;
    for (int i = 0; i < write_index; i++) {
      everything_ok = everything_ok && EEPROM.readChar(i) == write_buffer[i];
    }
    logger.println(LOG_TYPE::INFO, "Validating written Bytes...");
    logger.incIndent();
    if (everything_ok)
      logger.println(LOG_TYPE::INFO, "Successfull");
    else
      logger.println(LOG_TYPE::ERR, "Validation failed.");
    return everything_ok;
  }

public:

  static bool initEEPROM() {
    logger.println("Initializing EEPROM...");

    if (!EEPROM.begin(EEPROM_CONFIG_LEN_MAX)) {
      logger.println(LOG_TYPE::ERR, "failed to initialize EEPROM");
      return false;
    }
    return true;
  }

  static bool readConfig(char *buffer) {
    logger.println(LOG_TYPE::INFO, "Loading Config...");
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
      logger.println(LOG_TYPE::ERR, "Couldn't load: corrupted data.");
      logger.decIndent();
      return false;
    }
    if (!validateJson(buffer)) {
      logger.println(LOG_TYPE::ERR, "Couldn't load: invalid Json.");
      logger.decIndent();
      return false;
    }
    logger.println(LOG_TYPE::INFO, "Loading successfull");
    logger.decIndent();
    return true;
  };

  static bool writeConfig(const char *config_str) {
    logger.println(LOG_TYPE::INFO, "Validating...");
    logger.incIndent();
    if (strlen(config_str) > EEPROM_CONFIG_LEN_MAX) {
      logger.println(LOG_TYPE::ERR, "Cannot write config: too long");
      logger.decIndent();
      return false;
    }
    if (!validateJson(config_str)) {
      logger.println(LOG_TYPE::ERR, "Cannot write config: invalid JSON String");
      logger.decIndent();
      return false;
    }
    logger.println(LOG_TYPE::INFO, "Validation successfull");
    logger.decIndent();

    return writeConfigStr(config_str);
  };

  static bool readDefaultConfig(char *buffer) {
    logger.println(LOG_TYPE::INFO, "Loading Default Config");
    strcpy(buffer, default_config);
    return true;
  }

  static bool readHardConfig(char *buffer) {
    logger.println(LOG_TYPE::INFO, "Loading Hard Config");
    strcpy(buffer, json_str);
    return true;
  }
};
