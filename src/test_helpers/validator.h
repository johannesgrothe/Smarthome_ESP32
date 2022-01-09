#pragma once

// Use schemas from system submodule and the validator repo to create a validator class
// validator transforms arduinoJson into json objects and validates them
// https://github.com/pboettch/json-schema-validator

#include "ArduinoJson.h"
#include "json-patch.hpp"
#include "nlohmann/json-schema.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>


// Folder to load schemas from
constexpr char schema_folder[] = "src/system/json_schemas";

class custom_error_handler : public nlohmann::json_schema::basic_error_handler {
  void
  error(const nlohmann::json::json_pointer &ptr, const nlohmann::json &instance, const std::string &message) override {
    nlohmann::json_schema::basic_error_handler::error(ptr, instance, message);
    std::cerr << "ERROR: '" << ptr << "' - '" << instance << "': " << message << "\n";
  }
};

class Validator {
private:

  // Container for all loaded schemas
  std::vector<nlohmann::json> schemas_{};

  /**
   * Transforms a ArduinoJson into a nlohmann::json
   * @param input ArduinoJson to transform
   * @return Transformed nlohmann::json object
   */
  static nlohmann::json transform_json(const DynamicJsonDocument &input);

  /**
   * Searches the loaded schemas for one with the given name and returns it
   * @param schema_name Name of the schema to get
   * @return The schema with the selected name
   */
  nlohmann::json get_schema_for_name(const std::string &schema_name);

  /**
   * Reloads all schemas from the disk
   */
  void reload_schemas();


  // COPIED SHIT FROM HERE ON

  static void loader(const nlohmann::json_uri &uri, nlohmann::json &schema) {
    std::string filename = "./" + uri.path();
    std::ifstream lf(filename);
    if (!lf.good())
      throw std::invalid_argument("could not open " + uri.url() + " tried with " + filename);
    try {
      lf >> schema;
    } catch (const std::exception &e) {
      throw e;
    }
  }

  // COPIED SHIT END

public:
  Validator();

  bool validate(const DynamicJsonDocument &target, const std::string &schema_name);
};

nlohmann::json Validator::transform_json(const DynamicJsonDocument &input) {
  char buffer[15000]{};
  serializeJson(input, buffer);
  return nlohmann::json::parse(buffer);
}

Validator::Validator() {
  reload_schemas();
}

bool Validator::validate(const DynamicJsonDocument &target, const std::string &schema_name) {
  auto doc_json = transform_json(target);
  auto schema_json = get_schema_for_name(schema_name);
  nlohmann::json_schema::json_validator validator(loader,
                                                  nlohmann::json_schema::default_string_format_check);
  try {
    // insert this schema as the root to the validator
    // this resolves remote-schemas, sub-schemas and references via the given loader-function
    validator.set_root_schema(schema_json);
  } catch (const std::exception &e) {
    std::cerr << "setting root schema failed\n";
    std::cerr << e.what() << "\n";
  }

  custom_error_handler err;
  validator.validate(doc_json, err);

  if (err) {
    return false;
  }

  return true;
}

nlohmann::json Validator::get_schema_for_name(const std::string &schema_name) {
  std::stringstream err_strm;
  err_strm << "Schema with name '" << schema_name << "'does not exist.";
  throw err_strm.str().c_str();
}

void Validator::reload_schemas() {
  schemas_ = std::vector<nlohmann::json>();
}
