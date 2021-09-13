#pragma once

#include "ArduinoJson.h"
#include <vector>

/**
 * Method to check if json contains all of the selected keys.
 * @param json Json document to check for keys
 * @param key_list The list of all the keys that need to be present
 * @return Whether all keys were present
 */
static bool checkJsonForKeys(DynamicJsonDocument json, const std::vector<std::string> &key_list) {
  for (const auto &key: key_list) {
    if (!json.containsKey(key)) {
      return false;
    }
  }
  return true;
}