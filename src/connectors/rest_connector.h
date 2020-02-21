#ifndef __REST_Connector__
#define __REST_Connector__

#include <Arduino.h>
#include "../console_logger.h"
#include "../system_settings.h"
#include <WebServer.h>
#include <ESPmDNS.h>
#include "../../../../../.platformio/packages/framework-arduinoespressif32/libraries/WiFi/src/WiFiClient.h"


enum HTTP_TYPE {
  REQ_HTTP_GET, REQ_HTTP_POST, REQ_HTTP_UPDATE, REQ_HTTP_DELETE
};


//To  be done by the one and only Erich Honnecker!
// Gadget to communicate with REST Clients/Servers
class REST_Gadget {
protected:
  bool is_initialized;
  WiFiClient *momonga;
  WebServer *server;
  bool has_request;
  HTTP_TYPE type;
  char body[REST_MAX_BODY_LEN]{};
  char path[REST_MAX_PATH_LEN]{};


  void handleRoot() {
    server->send(200, "text/plain", "hello from esp8266!");
  }

  void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server->uri();
    message += "\nMethod: ";
    message += (server->method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server->args();
    message += "\n";
    for (uint8_t i = 0; i < server->args(); i++) {
      message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
    }
    server->send(404, "text/plain", message);
  }

  void initServer() {
    if (MDNS.begin("esp32")) {
      logger.println("MDNS responder started");
    }
    server = new WebServer(80);

    server->onNotFound(std::bind(&REST_Gadget::handleNotFound, this));
    server->begin();
    logger.println("HTTP server started");
  }

public:
  REST_Gadget() :
      is_initialized(false),
      momonga(nullptr),
      has_request(false),
      type(REQ_HTTP_GET) {
    initServer();

  };

  explicit REST_Gadget(WiFiClient *blubb) :
      is_initialized(true),
      momonga(blubb),
      has_request(false),
      type(REQ_HTTP_GET) {
    logger.println("Initializing REST_Gadget");
    logger.incIntent();
    initServer();
    logger.decIntent();
  };

  bool hasRequest() {
    bool buffer = has_request;
    has_request = false;
    return buffer;
  }

  HTTP_TYPE getRequestType() {
    return type;
  }

  const char *getRequestBody() {
    return &body[0];
  }

  const char *getRequestPath() {
    return &path[0];
  }

  void refresh() {
    server->handleClient();
  }
  
};

// Connector for REST Usage
class REST_Connector {
//protected:
//  bool decode_rest() {
//    Serial.println("[WARN] decode_rest() not implemented");
//    return false;
//  };
//
//  bool send_rest() {
//    Serial.println("[WARN] send_rest() not implemented");
//    return false;
//  };

public:
  REST_Connector();
};

#endif