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

class Validator {
private:

  // Container for all loaded schemas
  std::vector<std::tuple<std::string, nlohmann::json>> schemas_{};

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
  static void replace_refs(nlohmann::json &json, const std::vector<std::tuple<std::string, nlohmann::json>> &schemas);

public:
  Validator();

  bool validate(const DynamicJsonDocument &target, const std::string &schema_name);

  bool validate(const nlohmann::json &target, const std::string &schema_name);
};
