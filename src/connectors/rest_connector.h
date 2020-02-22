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

    REQUEST_TYPE req_type;
    if (server->method() == HTTP_GET) {
      req_type = REQ_HTTP_GET;
    } else if (server->method() == HTTP_POST) {
      req_type = REQ_HTTP_POST;
    } else if (server->method() == HTTP_DELETE) {
      req_type = REQ_HTTP_DELETE;
    } else if (server->method() == HTTP_PUT) {
      req_type = REQ_HTTP_PUT;
    } else {
      req_type = REQ_UNKNOWN;
    }
    setRequest(server->uri().c_str(), server->arg(0).c_str(), req_type);
    has_request = true;
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
      momonga(nullptr) {

  };

  REST_Gadget(JsonObject data, WiFiClient *blubb) :
      Request_Gadget(data),
      momonga(blubb) {
    logger.println("Initializing REST_Gadget");
    logger.incIntent();
    initServer();
    logger.decIntent();
    request_gadget_is_ready = true;
  };

  void refresh() override {
    if (!request_gadget_is_ready) {
      return;
    }
    server->handleClient();
  }

  /*TODO*/
  void sendRequest(REQUEST_TYPE req_type, IPAddress ip, const char *req_path, const char *req_body) override {
    logger.println(LOG_ERR, "To be Implemented");
  }

  void sendAnswer(const char *req_body, int status_code) override {
    server->send(status_code, "text/plain", req_body);
  }

  void sendAnswer(JsonObject req_body, int status_code) {
    logger.println(LOG_ERR, "To be Implemented");
  }


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