// Settings
#include "user_settings.h"
#include "system_settings.h"

// Network Gadgets
#include "connectors/ir_gadget.h"
#include "connectors/serial_gadget.h"
#include "connectors/radio_gadget.h"

// Gadget-Lib
#include "gadgets/gadget_library.h"

// Tools
#include "console_logger.h"
#include "system_timer.h"

// External Dependencies
#include "Client.h"
#include "ArduinoJson.h"

//#include "remotes/homebridge_remote.h"
#include "remotes/smarthome_gadget_remote.h"
#include "gadget_collection.h"
#include "system_storage.h"

#include "remotes/smarthome_code_remote.h"


#include "color.h"

static void rebootChip(const char *reason) {
  if (reason != nullptr) {
    logger.print("Rebooting Chip because: '");
    logger.add(reason);
    logger.add("' in ");
  } else {
    logger.println("Rebooting Chip in ");
  }
  for (byte k = 0; k < 5; k++) {
    logger.add(5 - k);
    logger.add(" ");
    delay(1000);
  }
  ESP.restart();
}

class SH_Main {
private:

  char client_name[CLIENT_NAME_LEN_MAX]{};

  IR_Gadget *ir_gadget;
  Radio_Gadget *radio_gadget;

  Request_Gadget *network_gadget;

  Gadget_Collection gadgets;

  CodeRemote *code_remote;

  GadgetRemote *gadget_remote;

  bool initGadgets(JsonArray gadget_json) {
    gadgets = Gadget_Collection();
    byte new_gadget_count = gadget_json.size() < MAIN_MAX_GADGETS ? gadget_json.size() : MAIN_MAX_GADGETS;
    logger.print(LOG_INFO, "Creating Gadgets: ");
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

  void mapConnectors(JsonObject connectors_json) {
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
          logger.println(LOG_DATA, gadget_name);
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
          logger.println(LOG_DATA, gadget_name);
        }
      }
      logger.decIndent();
    } else {
      logger.addln(" -");
    }
    logger.decIndent();
  }

  bool initConnectors(JsonObject connectors_json) {
    logger.print("Initializing Connectors: ");
    logger.addln(connectors_json.size());
    logger.incIndent();
    if (connectors_json["ir"] != nullptr) {
      ir_gadget = new IR_Gadget(connectors_json["ir"].as<JsonObject>());
    } else {
      ir_gadget = new IR_Gadget();
    }
    logger.decIndent();
    return true;
  }

  bool initNetwork(JsonObject json) {
    // check if JSON is valid
    if (json.isNull() || !json.containsKey("type")) {
      logger.println(LOG_ERR, "No valid network configuration.");
      return false;
    }

    // initialize Network
    if (strcmp(json["type"].as<char *>(), "mqtt") == 0) {
      network_gadget = new MQTT_Gadget(json["config"].as<JsonObject>());
    } else if (strcmp(json["type"].as<char *>(), "serial") == 0) {
      network_gadget = new Serial_Gadget(json["config"].as<JsonObject>());
    } else {
      logger.println(LOG_ERR, "Unknown Network Settings");
      return false;
    }
    logger.decIndent();
    return true;
  }

  void forwardCode(CodeCommand *com) {
    logger.incIndent();
    for (byte c = 0; c < gadgets.getGadgetCount(); c++) {
      gadgets.getGadget(c)->handleCodeUpdate(com->getCode());
    }
    logger.decIndent();
  }

  void handleCodeConnector(Code_Gadget *gadget) {
    if (gadget->hasNewCommand()) {
      CodeCommand *com = gadget->getCommand();
      logger.print("Command: ");
      logger.addln(com->getCode());

      if (code_remote != nullptr) {
        logger.incIndent();
        code_remote->handleNewCode(com);
        logger.decIndent();
      } else {
        forwardCode(com);
      }
    }
  }

  void handleCodeRemote() {
    if (code_remote == nullptr)
      return;
    if (code_remote->hasCode()) {
      CodeCommand *com = code_remote->getCode();
      forwardCode(com);
    }
  }

  void handleRequestConnector(Request_Gadget *gadget) {
    if (gadget->hasRequest()) {
      char type[REQUEST_TYPE_LEN_MAX]{};
      Request *req = gadget->getRequest();
      GADGET_TYPE g_type = gadget->getGadgetType();
      if (g_type == MQTT_G)
        strncpy(type, "MQTT", REQUEST_TYPE_LEN_MAX);
      else if (g_type == SERIAL_G)
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

  void handleSystemRequest(Request *req) {

    DynamicJsonDocument json_file(2048);
    DeserializationError err = deserializeJson(json_file, req->getBody());
    if (err != OK) {
      logger.print(LOG_ERR, "Broken System Command Received: Invalid JSON");
      return;
    }
    JsonObject json_body = json_file.as<JsonObject>();

    if (json_body["client_name"] == nullptr) {
      logger.print(LOG_ERR, "Broken System Command Received: 'client_name' missing");
      return;
    }

    if (strcmp(client_name, json_body["client_name"]) != 0) {
      // Command is not for me
      return;
    }

    logger.print("System Command Detected: ");
    logger.addln(req->getPath());
    logger.incIndent();

    if (strcmp(req->getPath(), "smarthome/from/sys/time") == 0) {
      if (json_body["time"] != nullptr) {
        unsigned long long new_time = json_body["time"].as<unsigned long long>();
//        system_timer.setTime(new_time);
      } else {
        logger.print(LOG_ERR, "Broken System Command Received: 'time' missing");
      }
    } else if (strcmp(req->getPath(), "smarthome/from/sys/command") == 0) {
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
        logger.print(LOG_ERR, "Broken System Command Received: 'command' missing");
      }
    } else if (strcmp(req->getPath(), "smarthome/from/sys/config/set") == 0) {

    } else {
      logger.println("Unknown Command");
    }
    logger.decIndent();
  }

  void handleRequest(Request *req) {
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
      }
    }
    logger.decIndent();
  }

  void updateGadgetRemote(const char *gadget_name, const char *service, const char *characteristic, int value) {
    gadget_remote->updateCharacteristic(gadget_name, service, characteristic, value);
  }

  bool initGadgetRemote(JsonObject json) {
    logger.println("Initializing Gadget Remote");
    logger.incIndent();

    if (json["type"] == nullptr) {
      logger.println(LOG_ERR, "'type' missing in gadget remote config");
    }
    if (json["gadgets"] == nullptr) {
      logger.println(LOG_ERR, "'gadgets' missing in gadget remote config");
    }
    auto remote_type = json["type"].as<const char *>();

    if (strcmp(remote_type, "smarthome") == 0) {
      JsonArray gadget_list = json["gadgets"].as<JsonArray>();
      if (gadget_list.size() > 0) {
        logger.println(LOG_DATA, "Smarthome");
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
        logger.println(LOG_DATA, "gadget-list is empty");
      }
    }

    logger.decIndent();
    return true;
  }

  bool initCodeRemote(JsonObject json) {
    logger.println("Initializing Code Remote");
    logger.incIndent();
    if (json.size() > 0) {
      auto *basic_remote = new SmarthomeCodeRemote(network_gadget, json);
      code_remote = basic_remote;
      logger.println(LOG_INFO, "OK");
    } else {
      logger.println(LOG_ERR, "Insufficient Configuration");
    }
    logger.decIndent();
    return true;
  }

  void testStuff() {
    logger.println("Testing Stuff");
    logger.incIndent();

    logger.decIndent();
  }

public:

  void init() {
    Serial.begin(SERIAL_SPEED);
    logger.println(LOG_INFO, "Launching...");
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
      logger.println(LOG_ERR, "No Name defined");
      strcpy(client_name, "TestClient");
    }

    if (json["gadgets"] != nullptr) {
      initGadgets(json["gadgets"]);
    } else {
      logger.println(LOG_ERR, "No gadgets-configuration found");
    }
    if (json["connector-mapping"] != nullptr) {
      mapConnectors(json["connector-mapping"]);
    } else {
      logger.println(LOG_ERR, "No connector-mapping-configuration found");
    }
    if (json["gadget-remote"] != nullptr) {
      initGadgetRemote(json["gadget-remote"]);
    } else {
      logger.println(LOG_ERR, "No remotes-configuration found");
    }
    if (json["code-remote"] != nullptr) {
      initCodeRemote(json["code-remote"]);
    } else {
      logger.println(LOG_ERR, "No code-remote-configuration found");
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
                logger.println(LOG_ERR, "Registering Client failed");
              }
            }
          }
        }
        delete resp;
      }
    }
    logger.addln();
  }

  void refresh() {
    handleRequestConnector(network_gadget);

    ir_gadget->refresh();
    handleCodeConnector(ir_gadget);

    handleCodeRemote();

    for (byte c = 0; c < gadgets.getGadgetCount(); c++) {
      gadgets.getGadget(c)->refresh();
    }
  }

  void refreshNetwork() {
    network_gadget->refresh();
  }

};

SH_Main smarthome_system;

static void mainTask(void *args) {
  while (true) {
    smarthome_system.refresh();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

static void networkTask(void *args) {
  while (true) {
    smarthome_system.refreshNetwork();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

static void createTasks() {
  xTaskCreatePinnedToCore(
    mainTask,     /* Task function. */
    "Smarthome_Main",       /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL,
    0);            /* Task handle. */

  xTaskCreatePinnedToCore(
    networkTask,     /* Task function. */
    "Smarthome_MQTT",       /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL,
    1);            /* Task handle. */
}

static void startSmarthomeSystem() {
  smarthome_system.init();
  createTasks();
}