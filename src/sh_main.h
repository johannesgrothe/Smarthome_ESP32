// Settings
#include "user_settings.h"
#include "system_settings.h"

// Network Gadgets
#include "connectors/ir_gadget.h"
#include "connectors/rest_gadget.h"
#include "connectors/serial_gadget.h"
#include "connectors/radio_gadget.h"

// Gadget-Lib
#include "gadgets/gadget_library.h"

// Tools
#include "console_logger.h"

// External Dependencies
#include "Client.h"
#include <WiFi.h>
#include "ArduinoJson.h"

#include "remotes/homebridge_remote.h"
#include "gadget_collection.h"
#include "system_storage.h"

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

  IR_Gadget *ir_gadget;
  MQTT_Gadget *mqtt_gadget;
  REST_Gadget *rest_gadget;
  Serial_Gadget *serial_gadget;
  Radio_Gadget *radio_gadget;

  WiFiClient network_client;

  Gadget_Collection gadgets;

  Remote *remotes[REMOTE_MANAGER_MAX_REMOTES]{};

  byte remote_count;

  unsigned long time_index = 0;

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
      buffergadget->initRemoteUpdate(std::bind(&SH_Main::updateRemotes, this, _1, _2, _3, _4));
      gadgets.addGadget(buffergadget);
      delay(500);
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

    if (connectors_json["mqtt"] != nullptr) {
      mqtt_gadget = new MQTT_Gadget(connectors_json["mqtt"].as<JsonObject>(), &network_client);
    } else {
      mqtt_gadget = new MQTT_Gadget();
    }

    if (connectors_json["rest"] != nullptr) {
      rest_gadget = new REST_Gadget(connectors_json["rest"].as<JsonObject>());
    } else {
      rest_gadget = new REST_Gadget();
    }

    if (connectors_json["serial"] != nullptr) {
      serial_gadget = new Serial_Gadget(connectors_json["serial"].as<JsonObject>());
    } else {
      DynamicJsonDocument json_file(50);
      JsonObject doc = json_file.as<JsonObject>();
      serial_gadget = new Serial_Gadget(doc);
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
    if (strcmp(json["type"].as<char *>(), "wifi") == 0) {
      logger.println("Creating Network: WiFi");
      logger.incIndent();
      network_client = WiFiClient();

      const char *ssid = json["config"]["ssid"].as<char *>();
      const char *passwd = json["config"]["password"].as<char *>();

      if (ssid == nullptr || passwd == nullptr) {
        logger.println(LOG_ERR, "Missing Username or Password.");
        return false;
      }

      logger.print(LOG_DATA, "");
      logger.add("Connecting to ");
      logger.add(ssid);

      byte connection_tries = 0;

      while (WiFiClass::status() != WL_CONNECTED && connection_tries < 6) {
        WiFi.begin(ssid, passwd);
        delay(1000);
        logger.add(".");
        connection_tries++;
      }
      logger.addln();
      if (WiFiClass::status() != WL_CONNECTED) {
        logger.println(LOG_DATA, "could not establish WiFi Connection...");
      } else {
        randomSeed(micros());
        logger.println(LOG_DATA, "WiFi connected");
        logger.print(LOG_DATA, "IP address: ");
        logger.addln(WiFi.localIP());
      }
    } else {
      logger.println(LOG_ERR, "Unknown Network Settings");
      return false;
    }
    logger.decIndent();
    return true;
  }

  void testStuff() {
    logger.println("Testing Stuff");
    logger.incIndent();

    //    rest_gadget->sendRequest(REQ_HTTP_POST, "text/plain", IPAddress(192, 168, 178, 111), 3005, "/irgendein/scheiss",
//                             "pennerus maximus schmongus");
    Color test_color = Color();
    test_color.setRGB(203, 80, 163);
    RGBColor *rgb_color = test_color.getRGB();
    HSLColor *hsl_color = test_color.getHSL();
    HSVColor *hsv_color = test_color.getHSV();
    Serial.println("\nsetRGB(203, 80, 163):");
    Serial.printf("RGB: %d, %d, %d \n", rgb_color->getRed(), rgb_color->getGreen(), rgb_color->getBlue());
    Serial.printf("HSL: %d, %d, %d \n", hsl_color->getHue(), hsl_color->getSaturation(), hsl_color->getLightness());
    Serial.printf("HSV: %d, %d, %d \n", hsv_color->getHue(), hsv_color->getSaturation(), hsv_color->getValue());

    Serial.println("\nsetHSL(126, 39, 29):");
    test_color.setHSL(126, 39, 29);
    rgb_color = test_color.getRGB();
    hsl_color = test_color.getHSL();
    hsv_color = test_color.getHSV();
    Serial.printf("RGB: %d, %d, %d \n", rgb_color->getRed(), rgb_color->getGreen(), rgb_color->getBlue());
    Serial.printf("HSL: %d, %d, %d \n", hsl_color->getHue(), hsl_color->getSaturation(), hsl_color->getLightness());
    Serial.printf("HSV: %d, %d, %d \n", hsv_color->getHue(), hsv_color->getSaturation(), hsv_color->getValue());

    Serial.println("\nsetHSV(0, 56, 80):");
    test_color.setHSV(0, 56, 80);
    rgb_color = test_color.getRGB();
    hsl_color = test_color.getHSL();
    hsv_color = test_color.getHSV();
    Serial.printf("RGB: %d, %d, %d \n", rgb_color->getRed(), rgb_color->getGreen(), rgb_color->getBlue());
    Serial.printf("HSL: %d, %d, %d \n", hsl_color->getHue(), hsl_color->getSaturation(), hsl_color->getLightness());
    Serial.printf("HSV: %d, %d, %d \n", hsv_color->getHue(), hsv_color->getSaturation(), hsv_color->getValue());
    Serial.println();

    logger.decIndent();
  }

  void initTime() {
    logger.println("Initializing Time");
    auto *req = new RestRequest(REQ_HTTP_GET, "/time", "",
                                3006, IPAddress(192, 168, 178, 108), "text/plain");
    rest_gadget->sendRequest(req);
    unsigned long start_time = millis();
    bool found_time = false;
    while (start_time + 3000 > millis() && !found_time) {
      if (rest_gadget->hasRequest()) {
        Request *res = rest_gadget->getRequest();
        if (strcmp(res->getPath(), "200") == 0) {
          found_time = true;
          time_index = strtol(res->getBody(), NULL, 10);
          logger.print("Got Time: ");
          logger.add(BASE_TIME);
          logger.addln(time_index);
        } else {
          logger.println("Received: ERR");
        }
      } else {
        rest_gadget->refresh();
      }
    }
    if (!found_time) {
      logger.println(LOG_ERR, "Cannot Sync Time");
    }
  }

  void handleCodeConnector(Code_Gadget *gadget) {
    if (gadget->hasNewCommand()) {
      unsigned long com = gadget->getCommand();
      logger.print("[HEX] Hex-Com: 0x");
      logger.addln(com, HEX);
      forwardCommand(com);
    }
  }

  void handleRequestConnector(Request_Gadget *gadget) {
    if (gadget->hasRequest()) {
      char type[REQUEST_TYPE_LEN_MAX]{};
      Request *req = gadget->getRequest();
      REQUEST_TYPE req_type = req->getType();
      if (req_type == REQ_UNKNOWN)
        strncpy(type, "<unknown>", REQUEST_TYPE_LEN_MAX);
      else if (req_type == REQ_HTTP_GET)
        strncpy(type, "GET", REQUEST_TYPE_LEN_MAX);
      else if (req_type == REQ_HTTP_POST)
        strncpy(type, "POST", REQUEST_TYPE_LEN_MAX);
      else if (req_type == REQ_HTTP_DELETE)
        strncpy(type, "DELETE", REQUEST_TYPE_LEN_MAX);
      else if (req_type == REQ_HTTP_PUT)
        strncpy(type, "PUT", REQUEST_TYPE_LEN_MAX);
      else if (req_type == REQ_HTTP_RESPONSE)
        strncpy(type, "RESPONSE", REQUEST_TYPE_LEN_MAX);
      else if (req_type == REQ_MQTT)
        strncpy(type, "MQTT", REQUEST_TYPE_LEN_MAX);
      else if (req_type == REQ_SERIAL)
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
      if (req->needsResponse()) {
        if (req->getType() == REQ_HTTP_GET || req->getType() == REQ_HTTP_POST || req->getType() == REQ_HTTP_DELETE ||
            req->getType() == REQ_HTTP_DELETE) {
          req->respond("404 GG", "unhandled");
        } else {
          req->respond("ERR", "unhandled");
        }
      }
      delete req;
    }
  }

  void forwardCommand(unsigned long code) {
    logger.incIndent();
    for (byte c = 0; c < gadgets.getGadgetCount(); c++) {
      gadgets.getGadget(c)->handleCodeUpdate(code);
    }
    logger.decIndent();
//    rest_gadget->sendRequest();
  }

  void handleStringRequest(REQUEST_TYPE type, const char *path, const char *body) {
    logger.println("Forwarding String-Request to Remotes:");
    logger.incIndent();
    forwardRequest(type, path, body);
    logger.decIndent();
  }

  void handleJsonRequest(REQUEST_TYPE type, const char *path, JsonObject body) {
    std::string str_path = path;
    logger.print("Forwarding Json-Request to ");
    logger.add(remote_count);
    logger.addln(" Remotes:");
    logger.incIndent();
    forwardRequest(type, path, body);
    logger.decIndent();
  }

  void handleSystemRequest(Request *req) {
    logger.print("System Command Detected: ");
    logger.addln(req->getPath());
    logger.incIndent();

    switch (req->getType()) {
      case REQ_HTTP_GET:
        logger.print("System / HTTP-GET", "");
        if (strcmp(req->getPath(), "sys/gadgets/list") == 0) {
          logger.addln("Asking for Gadget List");
          req->respond("200 OK", "gadgetlist");
        } else {
          logger.addln("<unknown>");
        }
        break;

      case REQ_HTTP_POST:
        logger.print("System / HTTP-POST", "");
        if (strcmp(req->getPath(), "sys/config/write") == 0) {
          logger.addln("Write config");
          if (System_Storage::writeConfig(req->getBody())) {
            req->respond("200 OK", "Writing config successfull");
          } else {
            req->respond("666 ERR", "Error Writing Config");
          }
        } else if (strcmp(req->getPath(), "sys/reboot") == 0) {
          logger.addln("Reboot");
          rebootChip(req->getBody());
        } else {
          logger.addln("<unknown>");
        }
        break;

      case REQ_MQTT:
        logger.print("System / MQTT", "");
        logger.addln("<unknown>");
        break;

      case REQ_SERIAL:
        logger.print("System / Serial", "");
        if (strcmp(req->getPath(), "sys/config/write") == 0) {
          logger.addln("Write config");
          if (System_Storage::writeConfig(req->getBody())) {
            req->respond("OK", "Writing config successfull");
          } else {
            req->respond("ERR", "Error Writing Config");
          }
        } else if (strcmp(req->getPath(), "sys/gadgets/list") == 0) {
          logger.addln("Asking for Gadget List");
          req->respond("OK", "gadgetlist");
        } else {
          logger.addln("<unknown>");
        }
        break;
      default:
        logger.print("System / UNHANDLED", "Unknown Request");
    }
    logger.decIndent();
  }

  void handleRequest(Request *req) {
    logger.incIndent();
    if (req->getBody() != nullptr) {
      unsigned int last_pos = strlen(req->getBody()) - 1;
      if (last_pos > 0) {
        std::string str_path = req->getPath();
        if (str_path.compare(0, 4, "sys/") == 0) {
          handleSystemRequest(req);
        } else {
          if (validateJson(req->getBody())) {
            DynamicJsonDocument json_file(2048);
            deserializeJson(json_file, req->getBody());
            JsonObject json_doc = json_file.as<JsonObject>();
            handleJsonRequest(req->getType(), req->getPath(), json_doc);
          } else {
            handleStringRequest(req->getType(), req->getPath(), req->getBody());
          }
        }
      }
    }
    logger.decIndent();
  }

  bool initRemotes(JsonObject json) {
    logger.println("Initializing Remotes");
    logger.incIndent();

    if (json["homebridge"] != nullptr) {
      JsonArray gadget_list = json["homebridge"].as<JsonArray>();
      if (gadget_list.size() > 0) {
        logger.println(LOG_DATA, "Homebridge");
        logger.incIndent();
        auto *homebridge_remote = new Homebridge_Remote(mqtt_gadget);
        for (auto &&gadget_name_str : gadget_list) {
          const char *gadget_name = gadget_name_str.as<const char *>();
          SH_Gadget *gadget = gadgets.getGadget(gadget_name);
          homebridge_remote->addGadget(gadget);
        }
        logger.decIndent();
        addRemote(homebridge_remote);
      } else {
        logger.println(LOG_DATA, "Homebridge-Configuration is empty");
      }
    }
    logger.decIndent();
    return true;
  }

  void updateRemotes(const char *gadget_name, const char *service, const char *characteristic, int value) {
    for (byte k = 0; k < remote_count; k++) {
      remotes[k]->updateCharacteristic(gadget_name, service, characteristic, value);
    }
  }

  void forwardRequest(REQUEST_TYPE type, const char *path, const char *body) {
    for (byte k = 0; k < remote_count; k++) {
      remotes[k]->handleRequest(path, type, body);
    }
  }

  void forwardRequest(REQUEST_TYPE type, const char *path, JsonObject body) {
    for (byte k = 0; k < remote_count; k++) {
      remotes[k]->handleRequest(path, type, body);
    }
  }

  void addRemote(Remote *new_remote) {
    if (remote_count < (REMOTE_MANAGER_MAX_REMOTES - 1)) {
      remotes[remote_count] = new_remote;
      remote_count++;
    }
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
    if (json["remotes"] != nullptr) {
      initRemotes(json["remotes"]);
    } else {
      logger.println(LOG_ERR, "No remotes-configuration found");
    }

    initTime();

    testStuff();

    logger.print("Free Heap: ");
    logger.add(ESP.getFreeHeap());
    logger.addln();
  }

  void refresh() {
    serial_gadget->refresh();
    handleCodeConnector(serial_gadget);
    handleRequestConnector(serial_gadget);

    ir_gadget->refresh();
    handleCodeConnector(ir_gadget);

    handleRequestConnector(mqtt_gadget);
    handleRequestConnector(rest_gadget);

    for (byte c = 0; c < gadgets.getGadgetCount(); c++) {
      gadgets.getGadget(c)->refresh();
    }
  }

  void refreshMQTT() {
    mqtt_gadget->refresh();
  }

  void refreshREST() {
    rest_gadget->refresh();
  }

};

SH_Main smarthome_system;

static void mainTask(void *args) {
  while (true) {
    smarthome_system.refresh();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

static void mqttTask(void *args) {
  while (true) {
    smarthome_system.refreshMQTT();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

static void restTask(void *args) {
  while (true) {
    smarthome_system.refreshREST();
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
    mqttTask,     /* Task function. */
    "Smarthome_MQTT",       /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL,
    1);            /* Task handle. */

  xTaskCreatePinnedToCore(
    restTask,     /* Task function. */
    "Smarthome_REST",       /* String with name of task. */
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