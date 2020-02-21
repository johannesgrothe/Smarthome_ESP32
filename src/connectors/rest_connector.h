#ifndef __REST_Connector__
#define __REST_Connector__

#include <WebServer.h>
#include <ESPmDNS.h>
#include "request_connector.h"


//To  be done by the one and only Erich Honnecker!
// Gadget to communicate with REST Clients/Servers
class REST_Gadget : public Request_Gadget {
protected:
  WiFiClient *momonga;
  WebServer *server;


  void handleRoot() {
    server->send(200, "text/plain", "hello from esp8266!");
  }

  void handleNotFound() {
    REQUEST_TYPE req_type = server->method() == HTTP_GET ? REQ_HTTP_GET : REQ_HTTP_POST;
    setRequest(server->uri().c_str(), server->arg(0).c_str(), req_type);
    has_request = true;
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
  /*TODO*/
  // class ServerHandler : public RequestHandler{
  /*
      bool canHandle(HTTPMethod method, String uri) {
        return true;
      }

      bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri) {
  //    doRobot( requestUri );
        handleNotFound();
        server.send(200, "text/plain", "Yes!!");
        return true;
      }

    };

    //    ServerHandler *handler = new ServerHandler;
  //    server->addHandler(&handler);
    */

  void initServer() {
    if (MDNS.begin("esp32")) {
      logger.println("MDNS responder started");
    }
    server = new WebServer(80);

    server->on("*", std::bind(&REST_Gadget::handleNotFound, this));
    server->onNotFound(std::bind(&REST_Gadget::handleNotFound, this));
    server->begin();
    logger.println("HTTP server started");
  }

public:
  REST_Gadget() :
      Request_Gadget(),
      momonga(nullptr){

  };

  REST_Gadget(JsonObject data, WiFiClient *blubb) :
      Request_Gadget(data),
      momonga(blubb){
    logger.println("Initializing REST_Gadget");
    logger.incIntent();
    initServer();
    logger.decIntent();
    is_initialized = true;
  };

  void refresh() override{
    if (!is_initialized) {
      return;
    }
    server->handleClient();
  }
  /*TODO*/
  // void send() {}
};

/*TODO*/
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