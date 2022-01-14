#include "validator.h"


nlohmann::json Validator::transform_json(const DynamicJsonDocument &input) {
  char buffer[15000]{};
  serializeJson(input, buffer);
  return nlohmann::json::parse(buffer);
}

Validator::Validator() {
  reload_schemas();
}

bool Validator::validate(const DynamicJsonDocument &target, const std::string &schema_name) {
  auto buf_json = transform_json(target);
  return validate(buf_json, schema_name);
}

bool Validator::validate(const nlohmann::json &target, const std::string &schema_name) {
  auto schema_json = get_schema_for_name(schema_name);
  nlohmann::json_schema::json_validator validator;
  try {
    validator.set_root_schema(schema_json);
  } catch (const std::exception &e) {
    logger_e("Validator", "Setting root schema failed: %s", e.what());
  }

  try {
    validator.validate(target);
  } catch (const std::exception &e) {
    logger_e("Validator", "Validation failed, here is why: %s", e.what());
    return false;
  }
  logger_i("Validator", "Validation using schema '%s' was successful", schema_name.c_str());
  return true;
}

nlohmann::json Validator::get_schema_for_name(const std::string &schema_name) {
  for (auto t: schemas_) {
    auto name = std::get<0>(t);
    if (name == schema_name) {
      return std::get<1>(t);
    }
  }
  logger_e("Validator", "Schema with name '%s' does not exist.", schema_name.c_str());
  throw std::exception();
}

void Validator::reload_schemas() {
  std::vector<std::tuple<std::string, nlohmann::json>> schemas;
  auto schema_paths = get_schema_files();
  for (const auto &path: schema_paths) {
    nlohmann::json buf_json;
    loader(path, buf_json);
    std::tuple<std::string, nlohmann::json> buf_tuple = {path, buf_json};
    schemas.push_back(buf_tuple);
  }

  // Replace refs
  for (auto &data: schemas) {
    nlohmann::json &json = std::get<1>(data);
    replace_refs(json, schemas);
  }

  std::vector<std::tuple<std::string, nlohmann::json>> buf_schemas;
  for (auto data: schemas) {
    auto path = std::get<0>(data);
    std::stringstream name_strm;
    for (char c: path) {
      name_strm << c;
      if (c == '/') {
        name_strm = std::stringstream();
      }
    }
    auto json = std::get<1>(data);
    std::tuple<std::string, nlohmann::json> buf_tuple(name_strm.str(), json);
    buf_schemas.push_back(buf_tuple);
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

void
Validator::replace_refs(nlohmann::json &json, const std::vector<std::tuple<std::string, nlohmann::json>> &schemas) {
  for (auto &it: json.items()) {
    if (it.value().is_object()) {
      if (it.value().contains("$ref")) {
        std::string replacement_name = it.value()["$ref"];
        bool found = false;
        for (auto t: schemas) {
          auto buf_name = std::get<0>(t);
          if (str_ends_with(buf_name, replacement_name)) {
            auto replacement_schema = std::get<1>(t);
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
