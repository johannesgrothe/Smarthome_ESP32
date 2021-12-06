#include "json_validator.h"

bool JsonValidator::checkKeys(const DynamicJsonDocument &json, const std::vector<std::string> &key_list) {
  for (const auto &key: key_list) {
    if (!json.containsKey(key)) {
      return false;
    }
  }
  return true;
}
