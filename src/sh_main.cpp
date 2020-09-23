#include <sstream>
#include "sh_main.h"

bool SH_Main::initGadgets(JsonArray gadget_json) {
  gadgets = Gadget_Collection();
  byte new_gadget_count = gadget_json.size() < MAIN_MAX_GADGETS ? gadget_json.size() : MAIN_MAX_GADGETS;
  logger.print(LOG_TYPE::INFO, "Creating Gadgets: ");
  logger.println(new_gadget_count);
  logger.incIndent();
  bool everything_ok = true;
  for (unsigned int pointer = 0; pointer < new_gadget_count; pointer++) {
    JsonObject gadget = gadget_json[pointer].as<JsonObject>();
    SH_Gadget *buffergadget = createGadget(gadget);
    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    using std::placeholders::_4;
    if (buffergadget != nullptr) {
      buffergadget->initRemoteUpdate(std::bind(&SH_Main::updateGadgetRemote, this, _1, _2, _3, _4));
      gadgets.addGadget(buffergadget);
    } else {
      everything_ok = false;
    }
  }
  logger.decIndent();
  return everything_ok;
}

void SH_Main::mapConnectors(JsonObject connectors_json) {
  // Mapping Code Connectors (IR/Radio) to the Gadgets for them to use
  logger.println("Mapping Connectors:");
  logger.incIndent();
  // IR
  logger.print("IR:");
  if (connectors_json["ir"] != nullptr && connectors_json["ir"].as<JsonArray>().size() > 0) {
    logger.printnl();
    logger.incIndent();
    JsonArray map_gadgets = connectors_json["ir"].as<JsonArray>();
    for (auto &&map_gadget : map_gadgets) {
      const char *gadget_name = map_gadget.as<const char *>();
      SH_Gadget *found_gadget = gadgets.getGadget(gadget_name);
      if (found_gadget != nullptr) {
        found_gadget->setIR(ir_gadget);
        logger.println(LOG_TYPE::DATA, gadget_name);
      }
    }
    logger.decIndent();
  } else {
    logger.println(" -");
  }

  // Radio
  logger.print("Radio:");
  if (connectors_json["radio"] != nullptr && connectors_json["radio"].as<JsonArray>().size() > 0) {
    logger.printnl();
    logger.incIndent();
    JsonArray map_gadgets = connectors_json["radio"].as<JsonArray>();
    for (auto &&map_gadget : map_gadgets) {
      const char *gadget_name = map_gadget.as<const char *>();
      SH_Gadget *found_gadget = gadgets.getGadget(gadget_name);
      if (found_gadget != nullptr) {
        found_gadget->setRadio(radio_gadget);
        logger.println(LOG_TYPE::DATA, gadget_name);
      }
    }
    logger.decIndent();
  } else {
    logger.println(" -");
  }
  logger.decIndent();
}

bool SH_Main::initConnectors(JsonObject connectors_json) {
  logger.println("Initializing Connectors: ");
  logger.incIndent();
  logger.println("Creating IR-Gadget: ");
  logger.incIndent();
  if (connectors_json["ir"] != nullptr) {
    ir_gadget = new IR_Gadget(connectors_json["ir"].as<JsonObject>());
  } else {
    ir_gadget = new IR_Gadget();
  }
  logger.decIndent();

  logger.println("Creating IR-Gadget:");
  logger.incIndent();
  if (connectors_json["mqtt"] != nullptr) {
    logger.println("Radio Configured bot not implemented");
  } else {
    logger.println("No Radio Configured");
  }
  logger.decIndent();

  logger.decIndent();
  return true;
}

bool SH_Main::initNetwork(const JsonObject &json) {
  // check if JSON is valid
  if (json.isNull() || !json.containsKey("type")) {
    logger.println(LOG_TYPE::ERR, "No valid network configuration.");
    return false;
  }

  // initialize Network
  if (json["type"].as<std::string>() == "mqtt") {
    network_gadget = new MQTT_Gadget(json["config"].as<JsonObject>());
  } else if (json["type"].as<std::string>() == "serial") {
    network_gadget = new Serial_Gadget(json["config"].as<JsonObject>());
  } else {
    logger.println(LOG_TYPE::ERR, "Unknown Network Settings");
    return false;
  }
  logger.decIndent();
  return true;
}

void SH_Main::handleCodeConnector(Code_Gadget *gadget) {
  if (gadget == nullptr) {
    return;
  }
  if (gadget->hasNewCommand()) {
    CodeCommand *com = gadget->getCommand();
    logger.print("Command: ");
    logger.println(com->getCode());

    if (code_remote != nullptr) {
      logger.incIndent();
      code_remote->handleNewCodeFromGadget(com);
      logger.decIndent();
    }
  }
}

void SH_Main::handleRequestConnector(Request_Gadget *gadget) {
  if (gadget == nullptr) {
    return;
  }
  if (gadget->hasRequest()) {
    std::string type;
    Request *req = gadget->getRequest();
    RequestGadgetType g_type = gadget->getGadgetType();
    if (g_type == RequestGadgetType::MQTT_G)
      type = "MQTT";
    else if (g_type == RequestGadgetType::SERIAL_G)
      type = "Serial";
    else
      type = "<o.O>";
    logger.printfln("[%s] '%s': %s", type.c_str(), req->getPath().c_str(), req->getBody().c_str());
    handleRequest(req);
    delete req;
  }
}

void SH_Main::handleSystemRequest(Request *req) {

  DynamicJsonDocument json_file(2048);
  DeserializationError err = deserializeJson(json_file, req->getBody());
  if (err != OK) {
    logger.print(LOG_TYPE::ERR, "Broken system command received: invalid json");
    return;
  }
  JsonObject json_body = json_file.as<JsonObject>();

  if (!eeprom_active_) {
    logger.print(LOG_TYPE::ERR, "EEPROM is broken, cannot deal with system requests.");
    return;
  }

  const std::string chip_id = System_Storage::readID();

  if (json_body.containsKey("receiver") && json_body["receiver"] != chip_id) {
    // Request is not for me
    return;
  }
  logger.println("System command received");

  // React to broadcast
  if (req->getPath() == "smarthome/broadcast/req" && json_body.containsKey("session_id")) {
    std::stringstream sstr;
    sstr << R"({"chip_id":")" << System_Storage::readID() << R"(", "session_id": )" << json_body["session_id"].as<int>() << "}";
    req->respond("smarthome/broadcast/res", sstr.str());
    return;
  }

  // All directed Requests
  if (json_body.containsKey("receiver")) {

    // Reset config
    if (req->getPath() == "smarthome/config/reset" && json_body.containsKey("session_id")) {
      System_Storage::writeTestEEPROM();
      System_Storage::resetContentFlag();
      std::stringstream sstr;
      sstr << R"({"ack":true)";
      sstr << R"(, "session_id": )" << json_body["session_id"].as<int>() << "}";
      req->respond("smarthome/config/write/id", sstr.str());
      return;
    }

    // Write parameters
    if (req->getPath() == "smarthome/config/write" && json_body.containsKey("param") && json_body.containsKey("value") && json_body.containsKey("session_id")) {
      auto param_name = json_body["param"].as<std::string>();
      auto param_val = json_body["value"].as<std::string>();
      bool write_successful = false;

      // write ID
      if (param_name == "id") {
        write_successful = System_Storage::writeID(param_val);
      }

      // Write Wifi SSID
      else if (param_name == "wifi_ssid") {
        write_successful = System_Storage::writeWifiSSID(param_val);
      }

      // Write Wifi PW
      else if (param_name == "wifi_pw") {
        write_successful = System_Storage::writeWifiPW(param_val);
      }

      // Write MQTT IP
      else if (param_name == "mqtt_ip") {
        write_successful = System_Storage::writeMQTTIP(param_val);
      }

      // Write MQTT Port
      else if (param_name == "mqtt_port") {
        write_successful = System_Storage::writeMQTTPort(param_val);
      }

      // Write MQTT User
      else if (param_name == "mqtt_user") {
//        write_successful = System_Storage::write(param_val);
      }

      // Write MQTT PW
      else if (param_name == "mqtt_pw") {
//        write_successful = System_Storage::write(param_val);
      }

      std::stringstream sstr;
      sstr << R"({"ack":)";
      if (write_successful) {
        sstr << "true";
      } else {
        sstr << "false";
      }
      sstr << R"(, "session_id": )" << json_body["session_id"].as<int>() << "}";
      req->respond("smarthome/config/write/id", sstr.str());
      return;
    }
  }
}

void SH_Main::handleRequest(Request *req) {
  logger.incIndent();
  if (!req->getBody().empty()) {
    std::string str_path = req->getPath();
    if (str_path.compare(0, 17,"smarthome/config/") == 0) {
      handleSystemRequest(req);
    } else if (str_path == "smarthome/broadcast/req") {
      handleSystemRequest(req);
    } else if (str_path == "smarthome/broadcast/res") {
      return; // cannot use broadcast responses
    } else {
      if (code_remote != nullptr) {
        code_remote->handleRequest(req);
      }
      if (gadget_remote != nullptr) {
        gadget_remote->handleRequest(req);
      }
    }
  } else {
    // Requests asking for content could have empty bodies
    logger.println(LOG_TYPE::ERR, "Empty Request");
  }
  logger.decIndent();
}

void SH_Main::updateGadgetRemote(const char *gadget_name, const char *service, const char *characteristic, int value) {
  gadget_remote->updateCharacteristic(gadget_name, service, characteristic, value);
}

bool SH_Main::initGadgetRemote(JsonObject json) {
  logger.println("Initializing Gadget Remote");
  logger.incIndent();

  if (json["type"] == nullptr) {
    logger.println(LOG_TYPE::ERR, "'type' missing in gadget remote config");
  }
  if (json["gadgets"] == nullptr) {
    logger.println(LOG_TYPE::ERR, "'gadgets' missing in gadget remote config");
  }
  auto remote_type = json["type"].as<const char *>();

  if (strcmp(remote_type, "smarthome") == 0) {
    JsonArray gadget_list = json["gadgets"].as<JsonArray>();
    if (gadget_list.size() > 0) {
      logger.println(LOG_TYPE::DATA, "Smarthome");
      logger.incIndent();
      auto *smarthome_remote = new SmarthomeGadgetRemote(network_gadget, json);
      for (auto &&gadget_name_str : gadget_list) {
        const char *gadget_name = gadget_name_str.as<const char *>();
        SH_Gadget *gadget = gadgets.getGadget(gadget_name);
        smarthome_remote->addGadget(gadget);
      }
      logger.decIndent();
      gadget_remote = smarthome_remote;
    } else {
      logger.println(LOG_TYPE::DATA, "gadget-list is empty");
    }
  }

  logger.decIndent();
  return true;
}

bool SH_Main::initCodeRemote(JsonObject json) {
  logger.println("Initializing Code Remote");
  logger.incIndent();

  if (json["type"] == nullptr) {
    logger.println(LOG_TYPE::ERR, "'type' missing in code remote config");
  }
  if (json["gadgets"] == nullptr) {
    logger.println(LOG_TYPE::ERR, "'gadgets' missing in code remote config");
  }
  auto remote_type = json["type"].as<const char *>();

  if (strcmp(remote_type, "smarthome") == 0) {
    JsonArray gadget_list = json["gadgets"].as<JsonArray>();
    if (gadget_list.size() > 0) {
      logger.println(LOG_TYPE::DATA, "Smarthome");
      logger.incIndent();
      auto *smarthome_remote = new SmarthomeCodeRemote(network_gadget, json);
      for (auto &&gadget_name_str : gadget_list) {
        const char *gadget_name = gadget_name_str.as<const char *>();
        SH_Gadget *gadget = gadgets.getGadget(gadget_name);
        smarthome_remote->addGadget(gadget);
      }
      logger.decIndent();
      code_remote = smarthome_remote;
    } else {
      logger.println(LOG_TYPE::DATA, "gadget-list is empty");
    }
  }

  logger.decIndent();
  return true;
}

void SH_Main::testStuff() {
  logger.println("Testing Stuff");
  logger.incIndent();

  if (eeprom_active_) {
    logger.println("testing eeprom:");

//    System_Storage::resetContentFlag();
//    System_Storage::writeTestEEPROM();
    logger.println(LOG_TYPE::DATA, System_Storage::readWholeEEPROM().c_str());
    logger.println(System_Storage::hasValidID());
    logger.println(System_Storage::hasValidWifiSSID());
    logger.println(System_Storage::hasValidWifiPW());
    logger.println(System_Storage::hasValidMQTTIP());
    logger.println(System_Storage::hasValidMQTTPort());

    logger.println(System_Storage::readID().c_str());
    logger.println(System_Storage::readWifiSSID().c_str());
    logger.println(System_Storage::readWifiPW().c_str());
    logger.println(System_Storage::readMQTTIP().c_str());
    logger.println(System_Storage::readMQTTPort().c_str());
  } else {
    logger.println(LOG_TYPE::FATAL, "eeprom isn't initialized");
  }

  logger.decIndent();
}

// TODO: deprecated, only load the parts of config that are needed
JsonObject loadConfig() {
  DynamicJsonDocument json_file(2048);
  deserializeJson(json_file, json_str); // Loads file from system_storage.h
  JsonObject json = json_file.as<JsonObject>();
  return json;
}

void SH_Main::init() {
  Serial.begin(SERIAL_SPEED);
  logger.println(LOG_TYPE::INFO, "Launching...");

  eeprom_active_ = System_Storage::initEEPROM();
  if (eeprom_active_) {
    logger.printfln("Client ID: '%s'", System_Storage::readID().c_str());
  }

  testStuff();

  logger.print(LOG_TYPE::INFO, "Boot Mode: ");
  system_mode = getBootMode();

  switch (system_mode) {
    case BootMode::Serial_Ony:
      logger.println("Serial Only");
      initModeSerial();
      break;
    case BootMode::Network_Only_FLASH:
      logger.println("Network Only: Flash");
      initModeNetwork(false);
      break;
    case BootMode::Network_Only_EEPROM:
      logger.println("Network Only: EEPROM");
      initModeNetwork(true);
      break;
    case BootMode::Full_Operation:
      logger.println("Full Operation");
      initModeComplete();
      break;
    default:
      logger.println("Unknown Boot Mode");
      break;
  }
  logger.printnl();
}

void SH_Main::initModeSerial() {
  DynamicJsonDocument json_file(2048);
  deserializeJson(json_file, F("{\"type\":\"serial\",\"config\"{}}"));
  initNetwork(json_file.as<JsonObject>());
}

void SH_Main::initModeNetwork(bool use_eeprom) {
  DynamicJsonDocument json_file(2048);
  JsonObject json;
  if (use_eeprom) {
    logger.println("Using EEPROM");
    json = loadConfig();
  } else {
    logger.println("Using FLASH");
#ifndef WIFI_SSID
#define WIFI_SSID "blub"
#endif

#ifndef WIFI_PW
#define WIFI_PW "blub"
#endif

#ifndef MQTT_IP
#define MQTT_IP "blub"
#endif

#ifndef MQTT_PORT
#define MQTT_PORT "1883"
#endif

    std::stringstream local_json_str;
    local_json_str << R"({"network":{"type":"mqtt","config":{"wifi_ssid":")" << std::string(WIFI_SSID)
                   << R"(","wifi_password":")" << std::string(WIFI_PW)
                   << R"(","ip":")" << std::string(MQTT_IP)
                   << R"(","port":)" << std::string(MQTT_PORT)
                   #ifdef MQTT_USERNAME
                   << R"(","mqtt_username":")" << MQTT_USERNAME << R"(")"
                   #else
                   << R"(,"mqtt_username":null)"
                   #endif
                   #ifdef MQTT_PW
                   << R"(,"mqtt_password":")" << MQTT_PW << R"(")"
                   #else
                   << R"(,"mqtt_password":null)"
                   #endif
                   << R"(}}})";

    DeserializationError e = deserializeJson(json_file, local_json_str.str());
    if (e != DeserializationError::Ok) {
      logger.println(LOG_TYPE::ERR, "Couldn't deserialize temporary json string");
    }
    logger.println(local_json_str.str().c_str());

    json = json_file.as<JsonObject>();

//    json_file["network"]["type"] = "mqtt";
//    char buf[300]{};
//    serializeJson(&buf[0], json_file);
//    logger.println(buf);
  }
  if (json.containsKey("network")) {
    initNetwork(json["network"]);
  } else {
    logger.println(LOG_TYPE::ERR, "Invalid config: no 'network' setting.");
  }
}

void SH_Main::initModeComplete() {
  JsonObject json = loadConfig();

  initNetwork(json["network"]);
  initConnectors(json["connectors"]);

  if (json["id"] != nullptr) {
    strncpy(client_name, json["id"].as<const char *>(), CLIENT_NAME_LEN_MAX);
  } else {
    logger.println(LOG_TYPE::ERR, "No Name defined");
    strcpy(client_name, "TestClient");
  }

  if (json["gadgets"] != nullptr) {
    initGadgets(json["gadgets"]);
  } else {
    logger.println(LOG_TYPE::ERR, "No gadgets-configuration found");
  }
  if (json["connector-mapping"] != nullptr) {
    mapConnectors(json["connector-mapping"]);
  } else {
    logger.println(LOG_TYPE::ERR, "No connector-mapping-configuration found");
  }
  if (json["gadget-remote"] != nullptr) {
    initGadgetRemote(json["gadget-remote"]);
  } else {
    logger.println(LOG_TYPE::ERR, "No remotes-configuration found");
  }
  if (json["code-remote"] != nullptr) {
    initCodeRemote(json["code-remote"]);
  } else {
    logger.println(LOG_TYPE::ERR, "No code-remote-configuration found");
  }

  testStuff();

  logger.print("Free Heap: ");
  logger.println(ESP.getFreeHeap());

  char client_str[50]{};
  unsigned long ident = micros() % 7023;
  snprintf(client_str, 50, R"({"request_id": %lu, "id": "%s"})", ident, client_name);
  network_gadget->sendRequest(new Request("smarthome/to/client/add", client_str));
  unsigned long const start_time = millis();
  while (start_time + 5000 > millis()) {
    if (!network_gadget->hasRequest()) {
      network_gadget->refresh();
    } else {
      Request *resp = network_gadget->getRequest();
      if (resp->getPath() == "smarthome/from/response" && getIdent(resp->getBody()) == ident) {
        DynamicJsonDocument time_json(2048);
        DeserializationError err = deserializeJson(time_json, resp->getBody());
        if (err == DeserializationError::Ok) {
          unsigned long time_offset = (millis() - start_time) / 2;
          JsonObject json_obj = time_json.as<JsonObject>();
          if (json_obj["ack"] != nullptr) {
            if (json_obj["ack"].as<bool>()) {
              logger.println("Adding Gadget succesfull.");
              if (json_obj["time"] != nullptr) {
                unsigned long long new_time = json_obj["time"].as<unsigned long long>();
                system_timer.setTime(new_time, time_offset);
              }
            } else {
              logger.println(LOG_TYPE::ERR, "Registering Client failed");
            }
          }
        }
      }
      delete resp;
    }
  }
}

void SH_Main::refresh() {
  switch (system_mode) {
    case BootMode::Serial_Ony:
      refreshModeSerial();
      break;
    case BootMode::Network_Only_EEPROM:
    case BootMode::Network_Only_FLASH:
      refreshModeNetwork();
      break;
    case BootMode::Full_Operation:
      refreshModeComplete();
      break;
    default:
      delay(1000);
      break;
  }
}

void SH_Main::refreshModeSerial() {
  handleRequestConnector(network_gadget);
}

void SH_Main::refreshModeNetwork() {
  handleRequestConnector(network_gadget);
}

void SH_Main::refreshModeComplete() {

  handleRequestConnector(network_gadget);

  ir_gadget->refresh();
  handleCodeConnector(ir_gadget);

//  handleCodeRemote();

  for (byte c = 0; c < gadgets.getGadgetCount(); c++) {
    gadgets.getGadget(c)->refresh();
  }
}

void SH_Main::refreshNetwork() {
  if (network_gadget == nullptr) {
    return;
  }
  network_gadget->refresh();
}

SH_Main::SH_Main() :
  ir_gadget(nullptr),
  radio_gadget(nullptr),
  network_gadget(nullptr),
  code_remote(nullptr),
  gadget_remote(nullptr),
  system_mode(BootMode::Unknown_Mode),
  last_req_id_(0),
  eeprom_active_(false){
}