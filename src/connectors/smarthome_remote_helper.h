#pragma once

static unsigned long getIdent(const char *json_str_input) {
  DynamicJsonDocument json_file(2048);
  DeserializationError err = deserializeJson(json_file, json_str_input);
  if (err == DeserializationError::Ok) {
    JsonObject json_obj = json_file.as<JsonObject>();
    if (json_obj["request_id"] != nullptr) {
      unsigned long ident = json_obj["request_id"].as<unsigned long>();
      return ident;
    }
  }
  return 0;
}

static bool getAck(const char *json_str_input) {
  DynamicJsonDocument json_file(2048);
  DeserializationError err = deserializeJson(json_file, json_str_input);
  if (err == DeserializationError::Ok) {
    JsonObject json_obj = json_file.as<JsonObject>();
    if (json_obj["ack"] != nullptr) {
      return json_obj["ack"].as<bool>();
    }
  }
  return false;
}
