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
#include <cstdio>
#include <dirent.h>
#include <unistd.h>

#include "../console_logger.h"


// Folder to load schemas from
constexpr char schema_folder[] = "./../../src/system/json_schemas";

// Error handler for the validator
class custom_error_handler : public nlohmann::json_schema::basic_error_handler {
  void
  error(const nlohmann::json::json_pointer &ptr, const nlohmann::json &instance, const std::string &message) override {
    nlohmann::json_schema::basic_error_handler::error(ptr, instance, message);
    std::stringstream err_strm;
    err_strm << "ERROR: '" << ptr << "' - '" << instance << "': " << message;
    logger_e("Validator", err_strm.str());
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

  /**
   * Loader function for validator. Copied from json-validator.cpp
   * @param uri Uri, whatever
   * @param schema Schema I guess
   */
  static void loader(const nlohmann::json_uri &uri, nlohmann::json &schema);

  /**
   * Gets all schema files from the schema directory
   * @return All schema file paths as vector
   */
  static std::vector<std::string> get_schema_files();

  /**
   * Checks if a string ends with the selected string
   * @param str String to check the ending of
   * @param ending Ending to check for
   * @return Whether the passed string ends with the selected one
   */
  static bool str_ends_with(const std::string &str, const std::string &ending);

  /**
   * Recursive function to replace all references in schemas
   * @param json Json to replace references in
   * @param schemas Schemas take replacement data from
   */
  static void replace_refs(nlohmann::json &json, const std::vector<std::tuple<std::string, nlohmann::json>>& schemas);

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
  char buffer[PATH_MAX];
  if (getcwd(buffer, sizeof(buffer)) != NULL) {
    printf("Current working directory : %s\n", buffer);
  } else {
    perror("getcwd() error");
  }

  reload_schemas();

  logger_i("Validator", "SCHEMAS:");
  for (const auto& schema: schemas_) {
    std::stringstream buf_str;
    buf_str << schema;
    logger_i("Validator", "%s", buf_str.str().c_str());
  }
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
    logger_e("Validator", "Setting root schema failed.");
    logger_e("Validator", e.what());
  }

  custom_error_handler err;
  validator.validate(doc_json, err);

  if (err) {
    return false;
  }

  return true;
}

nlohmann::json Validator::get_schema_for_name(const std::string &schema_name) {
  for (auto schema: schemas_) {
    if (schema["name"] == schema_name) {
      return schema;
    }
  }

  std::stringstream err_strm;
  err_strm << "Schema with name '" << schema_name << "'does not exist.";
  logger_e("Validator", err_strm.str());
  throw std::exception();
}

void Validator::reload_schemas() {
  logger_i("Validator", "Schemas:");
  std::vector<std::tuple<std::string, nlohmann::json>> schemas;
  auto schema_paths = get_schema_files();
  for (const auto &path: schema_paths) {
    nlohmann::json buf_json;
    loader(path, buf_json);
    std::tuple<std::string, nlohmann::json> buf_tuple = {path, buf_json};
    schemas.push_back(buf_tuple);
  }

  // Replace refs
  for (auto data: schemas) {
    nlohmann::json &json = std::get<1>(data);
    replace_refs(json, schemas);
  }

  std::vector<nlohmann::json> buf_schemas;
  for (auto data: schemas) {
    auto json = std::get<1>(data);
    buf_schemas.push_back(json);
  }

  schemas_ = buf_schemas;
}

void Validator::loader(const nlohmann::json_uri &uri, nlohmann::json &schema) {
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

std::vector<std::string> Validator::get_schema_files() {
  std::vector<std::string> out_tmp;

  struct dirent *entry = nullptr;
  DIR *dp = nullptr;
  dp = opendir(schema_folder);
  if (dp != nullptr) {
    while ((entry = readdir(dp))) {

      std::stringstream tmp_strm;
      tmp_strm << entry->d_name;
      auto tmp_str = tmp_strm.str();
      if (str_ends_with(tmp_str, ".json")) {
        std::stringstream path_strm;
        path_strm << schema_folder << "/" << tmp_str;
        out_tmp.push_back(path_strm.str());
        logger_i("Validator", "File Found: %s", tmp_str.c_str());
      }
    }
  }

  closedir(dp);
  return out_tmp;
}

bool Validator::str_ends_with(const std::string &str, const std::string &ending) {
  if (str.length() < ending.length()) {
    return false;
  }
  for (int i = 0; i < ending.length(); i++) {
    char first = str[str.length() - (i)];
    char second = ending[ending.length() - (i)];
    if (first != second) {
      return false;
    }
  }
  return true;
}

void Validator::replace_refs(nlohmann::json &json, const std::vector<std::tuple<std::string, nlohmann::json>>& schemas) {
  for (auto & it: json.items()) {
    if (it.value().is_object()) {
      if (it.value().contains("$ref")) {
        std::string replacement_name = it.value()["$ref"];
        logger_e("Validator", "Resolving ref: %s", replacement_name.c_str());
        bool found = false;
        for (auto t: schemas) {
          auto buf_name = std::get<0>(t);
          if (str_ends_with(buf_name, replacement_name)) {
            auto replacement_schema = std::get<1>(t);
            // TODO: replacement does not work
            json[it.key()] = replacement_schema;
            found = true;
            break;
          }
        }
        if (!found) {
          logger_e("Validator", "Reference could not be resolved");
          throw std::exception();
        }
      } else {
        replace_refs(it.value(), schemas);
      }
    }
  }
}
