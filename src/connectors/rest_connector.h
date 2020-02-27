#ifndef __REST_Connector__
#define __REST_Connector__

#include <WebServer.h>
#include <ESPmDNS.h>
#include "request_gadget.h"


//To  be done by the one and only Erich Honnecker!
// Gadget to communicate with REST Clients/Servers
class REST_Gadget : public Request_Gadget {
protected:
  WiFiClient *client;
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

  bool writeRespone(const char *response_str, int len) {
    response_status = 666;
    setResponsePath("test");
    setResponseBody(response_str);
    return true;
  }

  bool waitForResponse() {
    int len = client->available();
    delay(10);
    unsigned long time_now = millis();
    while(len < 0) {
      delay(10);
      if (millis() >= time_now + RESPONSE_MAX_WAIT_TIME) {
        has_response = false;
        return false;
      }
      len = client->available();
    }
    char buffer[REQUEST_BODY_LEN_MAX]{};
    if (len > REQUEST_BODY_LEN_MAX)
      len = REQUEST_BODY_LEN_MAX;
    for (int k = 0; k < len; k++) {
      buffer[k] = (char) client->read();
    }
    has_response = writeRespone(buffer, len);
    return has_response;
  }


  void sendAnswerFromServer(int status_code, const char *content_type, const char *req_body) {
    server->send(status_code, content_type, req_body);
  }

  bool
  sendRequestFromClient(REQUEST_TYPE req_type, const char *content_type, IPAddress ip, int port, const char *req_path,
                        const char *req_body) {
    logger.println("sending Request");
    char type[REQUEST_TYPE_LEN_MAX];
    if (req_type == REQ_HTTP_GET)
      strncpy(type, "GET", REQUEST_TYPE_LEN_MAX);
    else if (req_type == REQ_HTTP_POST)
      strncpy(type, "POST", REQUEST_TYPE_LEN_MAX);
    else if (req_type == REQ_HTTP_DELETE)
      strncpy(type, "DELETE", REQUEST_TYPE_LEN_MAX);
    else if (req_type == REQ_HTTP_PUT)
      strncpy(type, "PUT", REQUEST_TYPE_LEN_MAX);
    else
      return false;

    if (client->connect(ip, port)) {
      logger.print("connected to ");
      logger.addln(client->remoteIP());
      client->print(type);
      client->print(" ");
      client->print(req_path);
      client->println(" HTTP/1.1");
      client->println("User-Agent: ESP32_Smarthome");
      client->println("Connection: close");
      client->print("Content-Type: ");
      client->println(content_type);
      client->print("Content-Length: ");
      client->println(strlen(body));
      client->println();
      client->println(body);
      return waitForResponse();
    } else {
      return false;
    }

    return true;
  }

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
      Request_Gadget() {

  };

  REST_Gadget(JsonObject data, WiFiClient *gary) :
      Request_Gadget(data) {
    client = new WiFiClient;
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
//    int len = client->available();
//    if (len > 0) {
//      if (len > REQUEST_BODY_LEN_MAX) len = REQUEST_BODY_LEN_MAX;
//      for (int k = 0; k < len; k++){
//        response_body[k] = (char) client->read();
//      }
////      client->read(response_body, len);
//      has_response = true;
//
//      strcpy(response_path, "/test");
//      response_status = 200;
//    }
  }

  void sendRequest(REQUEST_TYPE req_type, const char *content_type, IPAddress ip, int port, const char *req_path,
                   const char *req_body) override {
    sendRequestFromClient(req_type, content_type, ip, port, req_path, req_body);
  }

  void sendRequest(REQUEST_TYPE req_type, const char *content_type, IPAddress ip, int port, const char *req_path,
                   JsonObject req_body) override {
    char body[REQUEST_BODY_LEN_MAX];
    serializeJson(req_body, &body[0], REQUEST_BODY_LEN_MAX);
    sendRequestFromClient(req_type, content_type, ip, port, req_path, body);
  }


  void sendAnswer(const char *req_body, int status_code) override {
    sendAnswerFromServer(status_code, "text/plain", req_body);
  }

  void sendAnswer(JsonObject req_body, int status_code) override {
    char body[REQUEST_BODY_LEN_MAX]{};
    serializeJson(req_body, &body[0], REQUEST_BODY_LEN_MAX);
    sendAnswerFromServer(status_code, "application/json", body);
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