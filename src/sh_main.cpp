#include <sstream>
#include <utility>
#include "sh_main.h"

bool SH_Main::initGadgets() {

  logger.print("Initializing Gadgets: ");

  auto eeprom_gadgets = System_Storage::readAllGadgets();

  logger.println(eeprom_gadgets.size());
  logger.incIndent();

  for (auto gadget: eeprom_gadgets) {
    auto gadget_ident = (GadgetIdentifier) std::get<0>(gadget);
    auto remote_bf = std::get<1>(gadget);
    auto pins = std::get<2>(gadget);
    auto name = std::get<3>(gadget);
    auto gadget_config_str = std::get<4>(gadget);
    auto code_config_str = std::get<5>(gadget);

    logger.printfln("Initializing %s", name.c_str());
    logger.incIndent();

    DynamicJsonDocument gadget_config(2500);
    DynamicJsonDocument code_config(2500);

    bool deserialization_ok = true;

    DeserializationError err;

    if (!gadget_config_str.empty()) {
      err = deserializeJson(gadget_config, gadget_config_str);
      if (err != DeserializationError::Ok) {
        deserialization_ok = false;
      }
    }

    if (!gadget_config_str.empty()) {
      err = deserializeJson(code_config, code_config_str);
      if (err != DeserializationError::Ok) {
        deserialization_ok = false;
      }
    }

    if (deserialization_ok) {
      logger.println(LOG_TYPE::DATA, "Creating Gadget");
      logger.incIndent();
      auto buf_gadget = createGadget(gadget_ident, pins, name, gadget_config.as<JsonObject>());
      logger.decIndent();

      if (buf_gadget != nullptr) {
        // Gadget Remote
        if (remote_bf[0]) {
          logger.println(LOG_TYPE::DATA, "Linking Gadget Remote");
          logger.incIndent();

          gadget_remote->addGadget(buf_gadget);

          using std::placeholders::_1;
          using std::placeholders::_2;
          using std::placeholders::_3;

          buf_gadget->setGadgetRemoteCallback(std::bind(&SH_Main::updateGadgetRemote, this, _1, _2, _3));

          logger.decIndent();
        }

        // Code Remote
        if (remote_bf[1]) {
          logger.println(LOG_TYPE::DATA, "Linking Code Remote");
          logger.incIndent();

          code_remote->addGadget(buf_gadget);

          logger.decIndent();

          for (int method_index = 0; method_index < GadgetMethodCount; method_index++) {
            std::stringstream ss;
            ss << method_index;
            std::string method_str = ss.str();
            if (code_config.containsKey(method_str)) {
              JsonArray codes = code_config[method_str].as<JsonArray>();
              for (int i = 0; i < codes.size(); i++) {
                unsigned long code = codes[i].as<unsigned long>();
                buf_gadget->setMethodForCode((GadgetMethod) method_index, code);
              }
            }
          }

          buf_gadget->printMapping();
        }

        // Event Remote
        if (remote_bf[2]) {
          logger.println(LOG_TYPE::DATA, "Linking Gadget Remote");
          logger.incIndent();
          // TODO: init event remote on gadgets
          logger.println(LOG_TYPE::ERR, "Not Implemented");

          logger.decIndent();
        }

        // IR Gadget
        bool ir_ok = true;
        if (gadgetRequiresIR(gadget_ident)) {
          if (ir_gadget != nullptr) {
            logger.println(LOG_TYPE::DATA, "Linking IR gadget");
            buf_gadget->setIR(ir_gadget);
          } else {
            logger.println(LOG_TYPE::ERR, "No IR gadget available");
            ir_ok = false;
          }
        } else {
          logger.println(LOG_TYPE::DATA, "No IR required");
        }

        // Radio
        // TODO: check when radio is implemented
        bool radio_ok = true;
        if (gadgetRequiresRadio(gadget_ident)) {
          if (radio_gadget != nullptr) {
            logger.println(LOG_TYPE::DATA, "Linking radio gadget");
            buf_gadget->setRadio(radio_gadget);
          } else {
            logger.println(LOG_TYPE::DATA, "No radio gadget available");
            radio_ok = false;
          }
        } else {
          logger.println(LOG_TYPE::DATA, "No radio required");
        }

        // Add created gadget to the list
        if (ir_ok && radio_ok) {
          gadgets.addGadget(buf_gadget);
        } else {
          logger.println(LOG_TYPE::DATA, "Gadget initialization failed due to ir/radio problems");
        }
      } else {
        logger.println(LOG_TYPE::ERR, "Gadget could not be created");
      }
    } else {
      logger.println(LOG_TYPE::ERR, "Error in config deserialization process");
    }
    logger.decIndent();
  }
  logger.decIndent();
  return true;
}

bool SH_Main::initConnectors() {
  logger.println("Initializing Connectors: ");

  int ir_recv = System_Storage::readIRrecvPin();
  int ir_send = System_Storage::readIRsendPin();
  int radio = 0;

  logger.println("Creating IR-Gadget: ");
  logger.incIndent();
  if (ir_recv || ir_send) {
    ir_gadget = std::make_shared<IR_Gadget>(ir_recv, ir_send);
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

bool SH_Main::initRemotes() {
  auto gadget_remote_mode = System_Storage::readGadgetRemote();
  auto code_remote_mode = System_Storage::readCodeRemote();
  auto event_remote_mode = System_Storage::readEventRemote();
  logger.println("Initializing remotes");
  logger.incIndent();

  // Init Gadget Remote
  if (gadget_remote_mode != GadgetRemoteMode::None) {
    logger.printfln("Initializing gadget remote");
    logger.incIndent();
    switch (gadget_remote_mode) {
      case GadgetRemoteMode::Smarthome:
        gadget_remote = make_shared<SmarthomeGadgetRemote>(network_gadget);
        break;
      default:
        logger.printfln(LOG_TYPE::ERR, "Could not initialize gadget remote (code %d)", (int) gadget_remote_mode);
    }
    logger.decIndent();
  } else {
    logger.printfln("No gadget remote configured");
  }

  // Init Code Remote
  if (code_remote_mode != CodeRemoteMode::None) {
    logger.printfln("Initializing code remote");
    logger.incIndent();
    switch (code_remote_mode) {
      case CodeRemoteMode::Smarthome:
        code_remote = make_shared<SmarthomeCodeRemote>(network_gadget);
        break;
      default:
        logger.printfln(LOG_TYPE::ERR, "Could not initialize code remote (code %d)", (int) code_remote_mode);
    }
    logger.decIndent();
  } else {
    logger.printfln("No code remote configured");
  }

  // Init Event Remote
  if (event_remote_mode != EventRemoteMode::None) {
    logger.printfln("Initializing event remote");
    logger.incIndent();
    switch (event_remote_mode) {
      case EventRemoteMode::Smarthome:
        // TODO: init event remote
//        event_remote = make_shared<SmarthomeCodeRemote>(network_gadget);
        logger.println(LOG_TYPE::ERR, "Event-Remote is not implemented");
        break;
      default:
        logger.printfln(LOG_TYPE::ERR, "Could not initialize event remote (code %d)", (int) code_remote_mode);
    }
    logger.decIndent();
  } else {
    logger.printfln("No event remote configured");
  }

  logger.decIndent();
  return true;
}


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

    network_gadget = std::make_shared<MQTT_Gadget>(client_id_,
                                                   ssid,
                                                   wifi_pw,
                                                   ip,
                                                   port,
                                                   user,
                                                   mqtt_pw);

  } else if (mode == NetworkMode::Serial) {
    network_gadget = std::make_shared<Serial_Gadget>();
  } else {
    logger.println(LOG_TYPE::ERR, "Unknown Network Settings");
    return false;
  }
  logger.decIndent();
  return true;
}

void SH_Main::handleCodeConnector(const std::shared_ptr<Code_Gadget> &gadget) {
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

void SH_Main::handleNetwork() {
  if (network_gadget == nullptr) {
    return;
  }
  if (network_gadget->hasRequest()) {
    std::string type;
    Request *req = network_gadget->getRequest();
    RequestGadgetType g_type = network_gadget->getGadgetType();
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
    req->respond(false);
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

  // system commands
  if (req->getPath() == "smarthome/sys") {
    auto subject = json_body["subject"].as<std::string>();
    if (subject == "reboot") {
      req->respond(true);
      auto payload = req->getPayload();
      if (payload.containsKey("message")) {
        rebootChip(payload["message"]);
      }
      rebootChip("Network Request");
    }
    req->respond(false);
    return;
  }

  // Reset config
  if (req->getPath() == "smarthome/config/reset") {
    // part of config to reset
    auto reset_option = json_body["reset_option"].as<std::string>();

    bool success = false;

    // reset complete config
    if (reset_option == "erase") {
      System_Storage::writeTestEEPROM();
      System_Storage::resetContentFlag();
      System_Storage::resetGadgets();
      success = true;
    }

      // reset the complete config
    else if (reset_option == "complete") {
      System_Storage::resetContentFlag();
      System_Storage::resetGadgets();
      success = true;
    }

      // reset the system config only
    else if (reset_option == "config") {
      System_Storage::resetContentFlag();
      success = true;
    }

      // reset the complete config
    else if (reset_option == "gadgets") {
      System_Storage::resetGadgets();
      success = true;
    }

    req->respond(success);
    return;
  }

  // Write parameters
  if (req->getPath() == "smarthome/config/write" && json_body.containsKey("param") && json_body.containsKey("value")) {
    // Parameter to write
    auto param_name = json_body["param"].as<std::string>();
    // Value to write as std::string
    auto param_val = json_body["value"].as<std::string>();
    // Value to write as uint8_t
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

      // Write radio receiver pin
    else if (param_name == "radio_recv_pin") {
      write_successful = System_Storage::writeRadioRecvPin(param_val_uint);
    }

      // Write radio sender pin
    else if (param_name == "radio_send_pin") {
      write_successful = System_Storage::writeRadioSendPin(param_val_uint);
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

      // read radio recv pin
    else if (param_name == "radio_recv_pin") {
      read_successful = true;
      read_val_uint = System_Storage::readRadioRecvPin();
    }

      // read radio send pin
    else if (param_name == "radio_send_pin") {
      read_successful = true;
      read_val_uint = System_Storage::readRadioSendPin();
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

  // Write gadget
  if (req->getPath() == "smarthome/gadget/add") {
    if (!json_body.containsKey("type") || !json_body.containsKey("name")) {
      req->respond(false);
      return;
    }

    auto type = json_body["type"].as<uint8_t>();

    auto name = json_body["name"].as<std::string>();

    uint8_t port0 = 0;
    uint8_t port1 = 0;
    uint8_t port2 = 0;
    uint8_t port3 = 0;
    uint8_t port4 = 0;

    if (json_body.containsKey("ports")) {
      JsonObject ports = json_body["ports"].as<JsonObject>();
      if (ports.containsKey("port0")) {
        port0 = ports["port0"].as<uint8_t>();
      }
      if (ports.containsKey("port1")) {
        port1 = ports["port1"].as<uint8_t>();
      }
      if (ports.containsKey("port2")) {
        port2 = ports["port2"].as<uint8_t>();
      }
      if (ports.containsKey("port3")) {
        port3 = ports["port3"].as<uint8_t>();
      }
      if (ports.containsKey("port4")) {
        port4 = ports["port4"].as<uint8_t>();
      }
    }

    pin_set pins = {port0, port1, port2, port3, port4};

    std::string gadget_config;
    std::string code_config;

    if (json_body.containsKey("config")) {
      gadget_config = json_body["config"].as<std::string>();
    }
    if (json_body.containsKey("codes")) {
      code_config = json_body["codes"].as<std::string>();
    }

    // Create bitfield
    bitfield_set remote_bf = {false, false, false, false, false, false, false, false};

    if (json_body.containsKey("remotes")) {
      JsonObject remote_json = json_body["remotes"].as<JsonObject>();
      if (remote_json.containsKey("gadget")) {
        remote_bf[0] = remote_json["gadget"].as<bool>();
      }
      if (remote_json.containsKey("code")) {
        remote_bf[1] = remote_json["code"].as<bool>();
      }
      if (remote_json.containsKey("event")) {
        remote_bf[2] = remote_json["event"].as<bool>();
      }
    }

    auto success_tuple = writeGadget(type, remote_bf, pins, name, gadget_config, code_config);
    bool success = std::get<0>(success_tuple);
    if (success) {
      req->respond(true);
    } else {
      auto err_msg = std::get<1>(success_tuple);
      req->respond(false, err_msg);
    }
    return;
  }

  req->respond(false);
}

void SH_Main::handleRequest(Request *req) {
  std::string req_path = req->getPath();
  if (!req->hasReceiver()) {
    req->updateReceiver(client_id_);
    for (const auto& list_path: broadcast_request_paths) {
      if (req_path == list_path) {
        // Handle broadcasts which do not have an receiver
        handleSystemRequest(req);
        return;
      }
    }
    return;
  } else if (client_id_ != req->getReceiver()) {
    // Return if the client is not the receiver of the message
    return;
  }

  // Check if the received request is a system request
  for (const auto& list_path: system_request_paths) {
    // TODO: check if last character is # and then only check 0 - (n-2)
    if (req_path.compare(0, list_path.length(), list_path) == 0) {
      handleSystemRequest(req);
      return;
    }
  }

  for (const auto& list_path: additional_request_paths) {
    if (req_path.compare(0, list_path.length(), list_path) == 0) {
      if (code_remote != nullptr) {
//        code_remote->handleRequest(req);
      }

      if (gadget_remote != nullptr) {
//        gadget_remote->handleRequest(req);
      }
      return;
    }
  }

  logger.printfln(LOG_TYPE::ERR, "Received request to unconfigured path");
}

void SH_Main::updateGadgetRemote(std::string gadget_name, GadgetCharacteristic characteristic, int value) {
  gadget_remote->updateCharacteristic(std::move(gadget_name), characteristic, value);
}

bool SH_Main::initGadgetRemote(JsonObject json) {
  logger.println("Initializing Gadget Remote");
  logger.incIndent();

  // TODO: fix gadget remote
//  if (json["type"] == nullptr) {
//    logger.println(LOG_TYPE::ERR, "'type' missing in gadget remote config");
//  }
//  if (json["gadgets"] == nullptr) {
//    logger.println(LOG_TYPE::ERR, "'gadgets' missing in gadget remote config");
//  }
//  auto remote_type = json["type"].as<const char *>();
//
//  if (strcmp(remote_type, "smarthome") == 0) {
//    JsonArray gadget_list = json["gadgets"].as<JsonArray>();
//    if (gadget_list.size() > 0) {
//      logger.println(LOG_TYPE::DATA, "Smarthome");
//      logger.incIndent();
//      auto *smarthome_remote = new SmarthomeGadgetRemote(network_gadget, json);
//      for (auto &&gadget_name_str : gadget_list) {
//        const char *gadget_name = gadget_name_str.as<const char *>();
//        SH_Gadget *gadget = gadgets.getGadget(gadget_name);
//        smarthome_remote->addGadget(gadget);
//      }
//      logger.decIndent();
//      gadget_remote = smarthome_remote;
//    } else {
//      logger.println(LOG_TYPE::DATA, "gadget-list is empty");
//    }
//  }

  logger.decIndent();
  return true;
}

bool SH_Main::initCodeRemote(JsonObject json) {
  logger.println("Initializing Code Remote");
  logger.incIndent();

  // TODO: fix code remote
//  if (json["type"] == nullptr) {
//    logger.println(LOG_TYPE::ERR, "'type' missing in code remote config");
//  }
//  if (json["gadgets"] == nullptr) {
//    logger.println(LOG_TYPE::ERR, "'gadgets' missing in code remote config");
//  }
//  auto remote_type = json["type"].as<const char *>();
//
//  if (strcmp(remote_type, "smarthome") == 0) {
//    JsonArray gadget_list = json["gadgets"].as<JsonArray>();
//    if (gadget_list.size() > 0) {
//      logger.println(LOG_TYPE::DATA, "Smarthome");
//      logger.incIndent();
//      auto *smarthome_remote = new SmarthomeCodeRemote(network_gadget, json);
//      for (auto &&gadget_name_str : gadget_list) {
//        const char *gadget_name = gadget_name_str.as<const char *>();
//        SH_Gadget *gadget = gadgets.getGadget(gadget_name);
//        smarthome_remote->addGadget(gadget);
//      }
//      logger.decIndent();
//      code_remote = smarthome_remote;
//    } else {
//      logger.println(LOG_TYPE::DATA, "gadget-list is empty");
//    }
//  }

  logger.decIndent();
  return true;
}

void SH_Main::testStuff() {
  logger.println("Testing Stuff");
  logger.incIndent();

//  System_Storage::printEEPROMLayout();
//
//  auto blub = (GadgetIdentifier) 55;
//
//  Serial.println((int) blub);
//
  if (eeprom_active_) {
    logger.println("testing eeprom:");

//    System_Storage::resetContentFlag();
//    System_Storage::writeTestEEPROM();

//    logger.println(LOG_TYPE::DATA, System_Storage::readWholeEEPROM().c_str());
    logger.println("Status-Byte:");
    logger.println(System_Storage::hasValidID());
    logger.println(System_Storage::hasValidWifiSSID());
    logger.println(System_Storage::hasValidWifiPW());
    logger.println(System_Storage::hasValidMQTTIP());
    logger.println(System_Storage::hasValidMQTTPort());
    logger.println(System_Storage::hasValidMQTTUsername());
    logger.println(System_Storage::hasValidMQTTPassword());
//    logger.println("IR:");
//    logger.println((int) System_Storage::readIRrecvPin());
//    logger.println((int) System_Storage::readIRsendPin());
//    logger.println("NetworkMode:");
//    logger.println((int) System_Storage::readNetworkMode());
//    logger.println("Remotes:");
//    logger.println((int) System_Storage::readGadgetRemote());
//    logger.println((int) System_Storage::readCodeRemote());
//    logger.println((int) System_Storage::readEventRemote());
//    logger.println("Config:");
//    logger.println(System_Storage::readID().c_str());
//    logger.println(System_Storage::readWifiSSID().c_str());
//    logger.println(System_Storage::readWifiPW().c_str());
//    logger.println(System_Storage::readMQTTIP().toString().c_str());
//    logger.println(System_Storage::readMQTTPort());
//    logger.println(System_Storage::readMQTTUsername().c_str());
//    logger.println(System_Storage::readMQTTPassword().c_str());

//    logger.println("Testing gadget saving:");

//    System_Storage::
//
//    (9, {true, true, false, false, false, false, false, false}, {120, 120, 120, 4, 5}, "blub_gadget", "{-_-}", "{0.0}");
//    System_Storage::writeGadget(5, {false, true, false, false, false, false, false, false}, {120, 120, 120, 4, 5}, "gggadget", "{o,O}", "{8#8}");
//    System_Storage::writeGadget(1, {true, false, false, false, false, false, false, false}, {120, 120, 120, 4, 5}, "yolokopterrrrrrr", "{yolokopterrrrrrrrrrrrrrrrrrrrrr}", "{RUMMMMSSSSSSSSSSSSSS}");

//    logger.println(LOG_TYPE::DATA, System_Storage::readWholeEEPROM().c_str());



//    auto g1 = System_Storage::readGadget(0);
//    auto g2 = System_Storage::readGadget(1);
//    auto g3 = System_Storage::readGadget(2);
//    auto g4 = System_Storage::readGadget(3);
//
//    Serial.println(int(std::get<0>(g1)));
//    auto remote_bf = std::get<1>(g1);
//    for (int i = 0; i < 3; i++) {
//      Serial.printf("%d, ", remote_bf[i]);
//    }
//    Serial.println("");
//    auto pins = std::get<2>(g1);
//    for (uint8_t pin : pins) {
//      Serial.printf("%d, ", (int) pin);
//    }
//    Serial.println("");
//    Serial.println(std::get<3>(g1).c_str());
//    Serial.println(std::get<4>(g1).c_str());
//    Serial.println(std::get<5>(g1).c_str());
//
//    Serial.println(int(std::get<0>(g2)));
//    remote_bf = std::get<1>(g2);
//    for (int i = 0; i < 3; i++) {
//      Serial.printf("%d, ", remote_bf[i]);
//    }
//    Serial.println("");
//    pins = std::get<2>(g2);
//    for (uint8_t pin : pins) {
//      Serial.printf("%d, ", (int) pin);
//    }
//    Serial.println("");
//    Serial.println(std::get<3>(g2).c_str());
//    Serial.println(std::get<4>(g2).c_str());
//    Serial.println(std::get<5>(g2).c_str());
//
//    Serial.println(int(std::get<0>(g3)));
//    remote_bf = std::get<1>(g3);
//    for (int i = 0; i < 3; i++) {
//      Serial.printf("%d, ", remote_bf[i]);
//    }
//    Serial.println("");
//    pins = std::get<2>(g3);
//    for (uint8_t pin : pins) {
//      Serial.printf("%d, ", (int) pin);
//    }
//    Serial.println("");
//    Serial.println(std::get<3>(g3).c_str());
//    Serial.println(std::get<4>(g3).c_str());
//    Serial.println(std::get<5>(g3).c_str());

    logger.println("Done");

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

  logger.printfln("EEPROM usage: %d / %d bytes\n", System_Storage::getEEPROMUsage(), EEPROM_SIZE);
  logger.printfln("Free Heap: %d", ESP.getFreeHeap());
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
    return;
  }

  initConnectors();

  initRemotes();

  initGadgets();

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
  // TODO: initialize time sync
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
  handleNetwork();
}

void SH_Main::refreshModeNetwork() {
  handleNetwork();
}

void SH_Main::refreshModeComplete() {

  handleNetwork();

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
