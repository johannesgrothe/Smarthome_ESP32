#include "sh_main.h"

bool SH_Main::initGadgets(JsonArray gadget_json) {
  gadgets = Gadget_Collection();
  byte new_gadget_count = gadget_json.size() < MAIN_MAX_GADGETS ? gadget_json.size() : MAIN_MAX_GADGETS;
  logger.print(LOG_TYPE::INFO, "Creating Gadgets: ");
  logger.addln(new_gadget_count);
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
    logger.addln();
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
    logger.addln(" -");
  }

  // Radio
  logger.print("Radio:");
  if (connectors_json["radio"] != nullptr && connectors_json["radio"].as<JsonArray>().size() > 0) {
    logger.addln();
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
    logger.addln(" -");
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

bool SH_Main::initNetwork(JsonObject json) {
  // check if JSON is valid
  if (json.isNull() || !json.containsKey("type")) {
    logger.println(LOG_TYPE::ERR, "No valid network configuration.");
    return false;
  }

  // initialize Network
  if (strcmp(json["type"].as<char *>(), "mqtt") == 0) {
    network_gadget = new MQTT_Gadget(json["config"].as<JsonObject>());
  } else if (strcmp(json["type"].as<char *>(), "serial") == 0) {
    network_gadget = new Serial_Gadget(json["config"].as<JsonObject>());
  } else {
    logger.println(LOG_TYPE::ERR, "Unknown Network Settings");
    return false;
  }
  logger.decIndent();
  return true;
}

void SH_Main::handleCodeConnector(Code_Gadget *gadget) {
  if (gadget->hasNewCommand()) {
    CodeCommand *com = gadget->getCommand();
    logger.print("Command: ");
    logger.addln(com->getCode());

    if (code_remote != nullptr) {
      logger.incIndent();
      code_remote->handleNewCodeFromGadget(com);
      logger.decIndent();
    }
  }
}

void SH_Main::handleRequestConnector(Request_Gadget *gadget) {
  if (gadget->hasRequest()) {
    char type[REQUEST_TYPE_LEN_MAX]{};
    Request *req = gadget->getRequest();
    RequestGadgetType g_type = gadget->getGadgetType();
    if (g_type == RequestGadgetType::MQTT_G)
      strncpy(type, "MQTT", REQUEST_TYPE_LEN_MAX);
    else if (g_type == RequestGadgetType::SERIAL_G)
      strncpy(type, "Serial", REQUEST_TYPE_LEN_MAX);
    else
      strncpy(type, "<o.O>", REQUEST_TYPE_LEN_MAX);

    logger.print("[");
    logger.add(type);
    logger.add("] '");
    logger.add(req->getPath());
    logger.add("' :");
    logger.addln(req->getBody());
    handleRequest(req);
    delete req;
  }
}

void SH_Main::handleSystemRequest(Request *req) {

  DynamicJsonDocument json_file(2048);
  DeserializationError err = deserializeJson(json_file, req->getBody());
  if (err != OK) {
    logger.print(LOG_TYPE::ERR, "Broken System Command Received: Invalid JSON");
    return;
  }
  JsonObject json_body = json_file.as<JsonObject>();

  if (json_body["client_name"] == nullptr) {
    logger.print(LOG_TYPE::ERR, "Broken System Command Received: 'client_name' missing");
    return;
  }

  if (strcmp(client_name, json_body["client_name"]) != 0) {
    // Command is not for me
    return;
  }

  logger.print("System Command Detected: ");
  logger.addln(req->getPath());
  logger.incIndent();

  if (strcmp(req->getPath(), "smarthome/from/sys/command") == 0) {
    if (json_body["command"] != nullptr) {
      const char *com = json_body["time"].as<const char *>();
      if (strcmp(com, "reboot") == 0) {
        if (json_body["message"] != nullptr) {
          const char *msg = json_body["message"].as<const char *>();
          rebootChip(msg);
        } else {
          rebootChip("No Reason given");
        }
      }
    } else {
      logger.print(LOG_TYPE::ERR, "Broken System Command Received: 'command' missing");
    }
  } else if (strcmp(req->getPath(), "smarthome/from/sys/config/set") == 0) {

  } else {
    logger.println("Unknown Command");
  }
  logger.decIndent();
}

void SH_Main::handleRequest(Request *req) {
  logger.incIndent();
  if (req->getBody() != nullptr) {
    unsigned int last_pos = strlen(req->getBody()) - 1;
    if (last_pos > 0) {
      std::string str_path = req->getPath();
      if (str_path.compare(0, 4, "smarthome/from/sys/") == 0) {
        handleSystemRequest(req);
      } else if (strcmp(req->getPath(), "smarthome/from/response") == 0) {
        logger.println("Ignoring unhandled response");
      } else {
        code_remote->handleRequest(req);
        gadget_remote->handleRequest(req);
      }
    } else {
      // Requests asking for content could have empty bodies
      logger.println(LOG_TYPE::ERR, "Empty Request");
    }
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

  logger.decIndent();
}

void SH_Main::init() {
  Serial.begin(SERIAL_SPEED);
  logger.println(LOG_TYPE::INFO, "Launching...");
  DynamicJsonDocument json_file(2048);
  bool eeprom_status = System_Storage::initEEPROM();

#ifndef USE_HARD_CONFIG
  char buffer[EEPROM_CONFIG_LEN_MAX]{};
  bool config_status = false;
  if (eeprom_status && System_Storage::readConfig(&buffer[0])) {
    config_status = (deserializeJson(json_file, &buffer[0]) == OK);
  }
  if (!config_status) {
    System_Storage::readDefaultConfig(buffer);
    config_status = (deserializeJson(json_file, &buffer[0]) == OK);
  }

  logger.decIndent();
#endif
#ifdef USE_HARD_CONFIG
  deserializeJson(json_file, json_str); // Loads file from system_storage.h
#endif
  JsonObject json = json_file.as<JsonObject>();

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
  logger.addln(ESP.getFreeHeap());

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
      if (strcmp(resp->getPath(), "smarthome/from/response") == 0 && getIdent(resp->getBody()) == ident) {
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
  logger.addln();
}

void SH_Main::refresh() {
  handleRequestConnector(network_gadget);

  ir_gadget->refresh();
  handleCodeConnector(ir_gadget);

//  handleCodeRemote();

  for (byte c = 0; c < gadgets.getGadgetCount(); c++) {
    gadgets.getGadget(c)->refresh();
  }
}

void SH_Main::refreshNetwork() {
  network_gadget->refresh();
}