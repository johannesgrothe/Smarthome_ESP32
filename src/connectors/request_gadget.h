#ifndef __Request_Gadget__
#define __Request_Gadget__

// External Imports
#include <ArduinoJson.h>
#include <cstring>
#include <IPAddress.h>
#include <WiFi.h>

// Other Imports
#include "../system_settings.h"
#include "../console_logger.h"

enum REQUEST_TYPE {
  REQ_UNKNOWN, REQ_HTTP_GET, REQ_HTTP_POST, REQ_HTTP_PUT, REQ_HTTP_DELETE, REQ_MQTT, REQ_SERIAL
};

class Request {
private:
  REQUEST_TYPE type;
  char body[REQUEST_BODY_LEN_MAX]{};
  char path[REQUEST_PATH_LEN_MAX]{};

public:
  Request(REQUEST_TYPE req_type, const char *req_path, const char *req_body) :
    type(req_type) {
    strcpy(path, req_path);
    strcpy(body, req_body);
  }

  REQUEST_TYPE getType() {
    return type;
  }

  const char *getPath() {
    return &path[0];
  }

  const char *getBody() {
    return &body[0];
  }
};

class Response {
private:
  int status_code;
  char body[REQUEST_BODY_LEN_MAX]{};

public:
  Response(int resp_code, const char *resp_body) :
    status_code(resp_code) {
    strcpy(body, resp_body);
  }

  int getStatus() {
    return status_code;
  }

  const char *getBody() {
    return &body[0];
  }
};

class Request_Gadget {
protected:
  bool request_gadget_is_ready;

  QueueHandle_t request_queue;
  QueueHandle_t response_queue;

  void addRequest(REQUEST_TYPE new_type, const char *new_path, const char *new_body) {
    auto *buffer_req = new Request(new_type, &new_path[0], &new_body[0]);
    xQueueSend(request_queue, &buffer_req, portMAX_DELAY);
  };

  void addResponse(int new_response_code, const char *new_response_body) {
    auto *buffer_res = new Response(new_response_code, &new_response_body[0]);
    xQueueSend(response_queue, &buffer_res, portMAX_DELAY);
  };

public:
  Request_Gadget() :
    request_gadget_is_ready(false) {
  }

  explicit Request_Gadget(JsonObject data) :
    request_gadget_is_ready(false) {
    request_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
    response_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Response *));
  }

  bool requestGadgetIsReady() {
    return request_gadget_is_ready;
  }

  bool hasRequest() {
    return uxQueueMessagesWaiting(request_queue) > 0;
//    return false;
  }

  Request *getRequest() {
    Request *bufreq;
    xQueueReceive(request_queue, &bufreq, portMAX_DELAY);
    return bufreq;
  }

  bool hasResponse() {
    return uxQueueMessagesWaiting(response_queue) > 0;
//    return false;
  }

  Response *getResponse() {
    Response *bufres;
    xQueueReceive(response_queue, &bufres, portMAX_DELAY);
    return bufres;
  }

  virtual void
  sendRequest(REQUEST_TYPE req_type, const char *content_type, IPAddress ip, int port, const char *req_path,
              const char *req_body) = 0;

  virtual void
  sendRequest(REQUEST_TYPE req_type, const char *content_type, IPAddress ip, int port, const char *req_path,
              JsonObject req_body) = 0;

  virtual void sendAnswer(const char *req_body, int status_code) = 0;

  virtual void sendAnswer(JsonObject req_body, int status_code) = 0;

  virtual void refresh() = 0;
};

#endif //__Request_Gadget__