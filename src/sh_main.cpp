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

bool SH_Main::initConnectors() {
  logger.println("Initializing Connectors: ");

  int ir_recv = System_Storage::readIRrecvPin();
  int ir_send = System_Storage::readIRsendPin();
  int radio = 0;

  logger.println("Creating IR-Gadget: ");
  logger.incIndent();
  if (ir_recv || ir_send) {
    ir_gadget = new IR_Gadget(ir_recv, ir_send);
  } else {
    logger.println("No IR Configured");
    ir_gadget = nullptr;
  }
  logger.decIndent();

  logger.println("Creating Radio-Gadget:");
  logger.incIndent();
  if (radio) {
    logger.println("Radio Configured bot not implemented");
  } else {
    logger.println("No Radio Configured");
    radio_gadget = nullptr;
  }
  logger.decIndent();

  logger.decIndent();
  return true;
}

/**
 * Initialized network to the given mode
 * @param mode
 * @return
 */
bool SH_Main::initNetwork(NetworkMode mode) {
  if (mode == NetworkMode::None) {
    logger.println(LOG_TYPE::ERR, "No network configured.");
    return false;
  }

  // initialize Network
  if (mode == NetworkMode::MQTT) {

    if (!System_Storage::hasValidWifiSSID()) {
      logger.println(LOG_TYPE::ERR, "Config has no valid wifi ssid");
      return false;
    }

    if (!System_Storage::hasValidWifiPW()) {
      logger.println(LOG_TYPE::ERR, "Config has no valid wifi password");
      return false;
    }

    if (!System_Storage::hasValidMQTTIP()) {
      logger.println(LOG_TYPE::ERR, "Config has no valid mqtt ip");
      return false;
    }

    if (!System_Storage::hasValidMQTTPort()) {
      logger.println(LOG_TYPE::ERR, "Config has no valid mqtt port");
      return false;
    }

    std::string ssid = System_Storage::readWifiSSID();
    std::string wifi_pw = System_Storage::readWifiPW();
    uint16_t port = System_Storage::readMQTTPort();
    IPAddress ip = System_Storage::readMQTTIP();
    std::string user = System_Storage::readMQTTUsername();
    std::string mqtt_pw = System_Storage::readMQTTPassword();

    network_gadget = new MQTT_Gadget(client_id_,
                                     ssid,
                                     wifi_pw,
                                     ip,
                                     port,
                                     user,
                                     mqtt_pw);

  } else if (mode == NetworkMode::Serial) {
    network_gadget = new Serial_Gadget();
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

  DynamicJsonDocument json_body = req->getPayload();

  if (!eeprom_active_) {
    logger.print(LOG_TYPE::ERR, "EEPROM is broken, cannot deal with system requests.");
    return;
  }

  logger.println("System command received");

  // React to broadcast
  if (req->getPath() == "smarthome/broadcast/req") {
    logger.println("Broadcast");
    DynamicJsonDocument doc(10);
    req->respond("smarthome/broadcast/res", doc);
    return;
  }

  // All directed Requests
  logger.println("Directed Request");

  // Reset config
  if (req->getPath() == "smarthome/config/reset") {
    System_Storage::writeTestEEPROM();
    System_Storage::resetContentFlag();
    req->respond(true);
    return;
  }

  // Write parameters
  if (req->getPath() == "smarthome/config/write" && json_body.containsKey("param") && json_body.containsKey("value")) {
    auto param_name = json_body["param"].as<std::string>();
    auto param_val = json_body["value"].as<std::string>();
    auto param_val_uint = json_body["value"].as<uint8_t>();

    logger.printfln("Write param '%s'", param_name.c_str());
    bool write_successful = false;

    // write ID
    if (param_name == "id") {
      write_successful = System_Storage::writeID(param_val);
      if (write_successful) {
        client_id_ = param_val;
      }
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
      if (param_val == "null") {
        write_successful = System_Storage::writeMQTTIP(IPAddress(0, 0, 0, 0));
      } else {
        IPAddress buf_ip;
        buf_ip.fromString(param_val.c_str());
        write_successful = System_Storage::writeMQTTIP(buf_ip);
      }
    }

      // Write MQTT Port
    else if (param_name == "mqtt_port") {

      write_successful = System_Storage::writeMQTTPort((uint16_t) atoi(param_val.c_str()));
    }

      // Write MQTT User
    else if (param_name == "mqtt_user") {
      write_successful = System_Storage::writeMQTTUsername(param_val);
    }

      // Write MQTT PW
    else if (param_name == "mqtt_pw") {
      write_successful = System_Storage::writeMQTTPassword(param_val);
    }

      // Write IR recv
    else if (param_name == "irrecv_pin") {
      write_successful = System_Storage::writeIRrecvPin(param_val_uint);
    }

      // Write IR send
    else if (param_name == "irsend_pin") {
      write_successful = System_Storage::writeIRsendPin(param_val_uint);
    }

      // Write radio pin
    else if (param_name == "radio_pin") {
      write_successful = System_Storage::writeRadioPin(param_val_uint);
    }

      // Write network mode
    else if (param_name == "network_mode") {
      if (param_val_uint < NetworkModeCount) {
        write_successful = System_Storage::writeNetworkMode((NetworkMode) param_val_uint);
      } else {
        write_successful = false;
      }
    }

      // Write gadget remote
    else if (param_name == "gadget_remote") {
      if (param_val_uint < GadgetRemoteModeCount) {
        write_successful = System_Storage::writeGadgetRemote((GadgetRemoteMode) param_val_uint);
      } else {
        write_successful = false;
      }
    }

      // Write code remote
    else if (param_name == "code_remote") {
      if (param_val_uint < CodeRemoteModeCount) {
        write_successful = System_Storage::writeCodeRemote((CodeRemoteMode) param_val_uint);
      } else {
        write_successful = false;
      }
    }

      // Write event remote
    else if (param_name == "event_remote") {
      if (param_val_uint < EventRemoteModeCount) {
        write_successful = System_Storage::writeEventRemote((EventRemoteMode) param_val_uint);
      } else {
        write_successful = false;
      }
    }

    req->respond(write_successful);

    if (param_name == "id" && write_successful) {
      client_id_ = param_val;
    }
    return;
  }

  // Read parameters
  if (req->getPath() == "smarthome/config/read" && json_body.containsKey("param")) {
    auto param_name = json_body["param"].as<std::string>();
    bool read_successful = false;
    std::string read_val_str;
    uint8_t read_val_uint;

    logger.printfln("Read param '%s'", param_name.c_str());

    // read wifi ssid
    if (param_name == "wifi_ssid") {
      read_successful = true;
      read_val_str = System_Storage::readWifiSSID();
    }

    // read mqtt ip
    if (param_name == "mqtt_ip") {
      read_successful = true;
      std::stringstream sstr;
      sstr << System_Storage::readMQTTIP().toString().c_str();
      read_val_str = sstr.str();
    }

    // read mqtt port
    if (param_name == "mqtt_port") {
      read_successful = true;
      std::stringstream sstr;
      sstr << System_Storage::readMQTTPort();
      read_val_str = sstr.str();
    }

    // read mqtt username
    if (param_name == "mqtt_user") {
      read_successful = true;
      read_val_str = System_Storage::readMQTTUsername();
    }

    // send response if read was successful
    if (read_successful) {
      DynamicJsonDocument doc(100);
      doc["value"] = read_val_str;
      req->respond(doc);
      return;
    }

    // read irrecv pin
    if (param_name == "irrecv_pin") {
      read_successful = true;
      read_val_uint = System_Storage::readIRrecvPin();
    }

      // read irsend pin
    else if (param_name == "irsend_pin") {
      read_successful = true;
      read_val_uint = System_Storage::readIRsendPin();
    }

      // read radio pin
    else if (param_name == "radio_pin") {
      read_successful = true;
      read_val_uint = System_Storage::readRadioPin();
    }

      // read network mode
    else if (param_name == "network_mode") {
      read_successful = true;
      read_val_uint = (uint8_t) System_Storage::readNetworkMode();
    }

      // read gadget remote
    else if (param_name == "gadget_remote") {
      read_successful = true;
      read_val_uint = (uint8_t) System_Storage::readGadgetRemote();
    }

      // read code remote
    else if (param_name == "code_remote") {
      read_successful = true;
      read_val_uint = (uint8_t) System_Storage::readCodeRemote();
    }

      // read event_remote
    else if (param_name == "event_remote") {
      read_successful = true;
      read_val_uint = (uint8_t) System_Storage::readEventRemote();
    }

    // send response if read was successful
    if (read_successful) {
      DynamicJsonDocument doc(100);
      doc["value"] = read_val_uint;
      req->respond(doc);
      return;
    }

  }
  req->dontRespond();
}

void SH_Main::handleRequest(Request *req) {
  std::string req_path = req->getPath();
  if (!req->hasReceiver()) {
    req->updateReceiver(client_id_);
    if (req_path == "smarthome/broadcast/req") {
      // Handle broadcasts which do not have an receiver
      handleSystemRequest(req);
    }
    return;
  } else if (req->hasReceiver() && client_id_ != req->getReceiver()) {
    // Return if the client is not the receiver of the message
    return;
  }

  if (req_path.compare(0, 17, "smarthome/config/") == 0) {
    handleSystemRequest(req);
    return;
  }

  if (code_remote != nullptr) {
    code_remote->handleRequest(req);
  }
  if (gadget_remote != nullptr) {
    gadget_remote->handleRequest(req);
  }
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

  System_Storage::printEEPROMLayout();

  if (eeprom_active_) {
    logger.println("testing eeprom:");

//    System_Storage::resetContentFlag();
//    System_Storage::writeTestEEPROM();
    logger.println(LOG_TYPE::DATA, System_Storage::readWholeEEPROM().c_str());
    logger.println("Status-Byte:");
    logger.println(System_Storage::hasValidID());
    logger.println(System_Storage::hasValidWifiSSID());
    logger.println(System_Storage::hasValidWifiPW());
    logger.println(System_Storage::hasValidMQTTIP());
    logger.println(System_Storage::hasValidMQTTPort());
    logger.println(System_Storage::hasValidMQTTUsername());
    logger.println(System_Storage::hasValidMQTTPassword());
    logger.println("IR:");
    logger.println((int) System_Storage::readIRrecvPin());
    logger.println((int) System_Storage::readIRsendPin());
    logger.println("NetworkMode:");
    logger.println((int) System_Storage::readNetworkMode());
    logger.println("Remotes:");
    logger.println((int) System_Storage::readGadgetRemote());
    logger.println((int) System_Storage::readCodeRemote());
    logger.println((int) System_Storage::readEventRemote());
    logger.println("Config:");
    logger.println(System_Storage::readID().c_str());
    logger.println(System_Storage::readWifiSSID().c_str());
    logger.println(System_Storage::readWifiPW().c_str());
    logger.println(System_Storage::readMQTTIP().toString().c_str());
    logger.println(System_Storage::readMQTTPort());
    logger.println(System_Storage::readMQTTUsername().c_str());
    logger.println(System_Storage::readMQTTPassword().c_str());
  } else {
    logger.println(LOG_TYPE::FATAL, "eeprom isn't initialized");
  }

  logger.decIndent();
}

void SH_Main::init() {
  Serial.begin(SERIAL_SPEED);
  logger.println(LOG_TYPE::INFO, "Launching...");

  eeprom_active_ = System_Storage::initEEPROM();
  if (eeprom_active_) {
    client_id_ = System_Storage::readID();
    logger.printfln("Client ID: '%s'", client_id_.c_str());
  }

  testStuff();

  logger.print(LOG_TYPE::INFO, "Boot Mode: ");
  system_mode = getBootMode();

  switch (system_mode) {
    case BootMode::Serial_Ony:
      logger.println("Serial Only");
      initModeSerial();
      break;
    case BootMode::Network_Only_EEPROM:
      logger.println("Network Only: EEPROM");
      initModeNetwork();
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

  logger.print("Free Heap: ");
  logger.println(ESP.getFreeHeap());
}

/**
 * Initializes the chip in network only mode using the serial network
 */
void SH_Main::initModeSerial() {
  bool status = initNetwork(NetworkMode::Serial);
  if (status) {
    logger.println("Serial network initialized");
  } else {
    logger.println(LOG_TYPE::ERR, "serial network was not initialized");
  }
}

/**
 * Initializes the chip in network only mode using the network mode loaded from eeprom
 */
void SH_Main::initModeNetwork() {
  if (eeprom_active_) {
    auto mode = System_Storage::readNetworkMode();
    initNetwork(mode);
  } else {
    logger.println(LOG_TYPE::ERR, "network type could not be loaded");
  }
}

/**
 * Initializes the chip with all gadgets and connectors loaded
 */
void SH_Main::initModeComplete() {
  if (eeprom_active_) {
    auto mode = System_Storage::readNetworkMode();
    initNetwork(mode);
  } else {
    logger.println(LOG_TYPE::ERR, "network type could not be loaded");
  }

  initConnectors();

//  if (json["gadgets"] != nullptr) {
//    initGadgets(json["gadgets"]);
//  } else {
//    logger.println(LOG_TYPE::ERR, "No gadgets-configuration found");
//  }
//  if (json["connector-mapping"] != nullptr) {
//    mapConnectors(json["connector-mapping"]);
//  } else {
//    logger.println(LOG_TYPE::ERR, "No connector-mapping-configuration found");
//  }
//  if (json["gadget-remote"] != nullptr) {
//    initGadgetRemote(json["gadget-remote"]);
//  } else {
//    logger.println(LOG_TYPE::ERR, "No remotes-configuration found");
//  }
//  if (json["code-remote"] != nullptr) {
//    initCodeRemote(json["code-remote"]);
//  } else {
//    logger.println(LOG_TYPE::ERR, "No code-remote-configuration found");
//  }

  char client_str[50]{};
  unsigned long ident = micros() % 7023;
//  snprintf(client_str, 50, R"({"request_id": %lu, "id": "%s"})", ident, client_name);
//  network_gadget->sendRequest(new Request("smarthome/to/client/add", client_str));
  unsigned long const start_time = millis();
//  while (start_time + 5000 > millis()) {
//    if (!network_gadget->hasRequest()) {
//      network_gadget->refresh();
//    } else {
//      Request *resp = network_gadget->getRequest();
//      if (resp->getPath() == "smarthome/from/response" && getIdent(resp->getPayload()) == ident) {
//        DynamicJsonDocument time_json(2048);
//        DeserializationError err = deserializeJson(time_json, resp->getPayload());
//        if (err == DeserializationError::Ok) {
//          unsigned long time_offset = (millis() - start_time) / 2;
//          JsonObject json_obj = time_json.as<JsonObject>();
//          if (json_obj["ack"] != nullptr) {
//            if (json_obj["ack"].as<bool>()) {
//              logger.println("Adding Gadget succesfull.");
//              if (json_obj["time"] != nullptr) {
//                unsigned long long new_time = json_obj["time"].as<unsigned long long>();
//                system_timer.setTime(new_time, time_offset);
//              }
//            } else {
//              logger.println(LOG_TYPE::ERR, "Registering Client failed");
//            }
//          }
//        }
//      }
//      delete resp;
//    }
//  }
}

void SH_Main::refresh() {
  switch (system_mode) {
    case BootMode::Serial_Ony:
      refreshModeSerial();
      break;
    case BootMode::Network_Only_EEPROM:
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
  client_id_("null"),
  ir_gadget(nullptr),
  radio_gadget(nullptr),
  network_gadget(nullptr),
  code_remote(nullptr),
  gadget_remote(nullptr),
  system_mode(BootMode::Unknown_Mode),
  last_req_id_(0),
  eeprom_active_(false) {
}