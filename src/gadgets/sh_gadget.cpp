#include "sh_gadget.h"

void SH_Gadget::updateCharacteristic(const char *characteristic, int value) {
  updateRemotes(&name[0], &name[0], characteristic, value);
}

const char *SH_Gadget::findMethodForCode(unsigned long code) {
  for (byte k = 0; k < mapping_count; k++) {
    if (mapping[k]->containsCode(code)) {
      const char *method_name = mapping[k]->getName();
      logger.print(name, "-> ");
      logger.addln(method_name);
      return method_name;
    }
  }
  return nullptr;
}

void SH_Gadget::handleMethodUpdate(const char *method) {

}

SH_Gadget::SH_Gadget() :
  remoteInitialized(false),
  name("default"),
  initialized(false),
  has_changed(true),
  type(GadgetType::None) {};

SH_Gadget::SH_Gadget::SH_Gadget(const JsonObject& gadget, const GadgetType gadget_type) :
  remoteInitialized(false),
  initialized(false),
  has_changed(true),
  type(gadget_type) {
  if (gadget["name"] != nullptr) {
    byte namelen =
      strlen(gadget["name"].as<const char *>()) < GADGET_NAME_LEN_MAX ? strlen(gadget["name"].as<const char *>())
                                                                      : GADGET_NAME_LEN_MAX;
    strncpy(name, gadget["name"].as<const char *>(), namelen);
  } else {
    strcpy(name, "Unknown");
    logger.println(LOG_TYPE::ERR, "No Name found!");
  }
  if (gadget["mapping"] != nullptr) {
    JsonObject local_mapping = gadget["mapping"].as<JsonObject>();
    mapping_count = local_mapping.size() < MAPPING_MAX_COMMANDS ? local_mapping.size() : MAPPING_MAX_COMMANDS;
    logger.print(LOG_TYPE::INFO, "Configuring Mapping, Commands: ");
    logger.addln(mapping_count);
    logger.incIndent();
    byte j = 0;
    for (auto &&com : local_mapping) {
      if (j < mapping_count) {
        const char *new_name = com.key().c_str();
        JsonArray buf_arr = com.value().as<JsonArray>();
        mapping[j] = new Mapping_Reference(buf_arr, new_name);
        j++;
      }
    }
    logger.decIndent();
    logger.println("Method Mapping loaded.");
  } else {
    logger.println(LOG_TYPE::WARN, "No Mapping Found.");
  }
}

void SH_Gadget::initRemoteUpdate(std::function<void(const char *, const char *, const char *, int)> update_method) {
  updateRemotes = update_method;
  logger.println("Initialized Callbacks");
  remoteInitialized = true;
}

GadgetType SH_Gadget::getType() {
  return type;
}

const char *SH_Gadget::getName() {
  return &name[0];
}

bool SH_Gadget::isInitialized() {
  return initialized;
}

void SH_Gadget::handleCodeUpdate(unsigned long code) {
  const char *method_name = findMethodForCode(code);
  if (method_name != nullptr) {
    logger.print(name, "Applying Method: ");
    logger.addln(method_name);
    logger.incIndent();
    handleMethodUpdate(method_name);
    logger.decIndent();
  } else {
    logger.print(name, "No Method for '");
    logger.add(code);
    logger.addln("' found.");
  }
}

void SH_Gadget::printMapping() {
  logger.print(name, "Accessible Methods: ");
  logger.addln(mapping_count);
  logger.incIndent();
  for (byte k = 0; k < mapping_count; k++) {
    mapping[k]->printMapping();
  }
  logger.decIndent();
}