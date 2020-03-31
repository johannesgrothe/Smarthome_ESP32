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
  unsigned int identifier{};

public:
  Request(REQUEST_TYPE req_type, const char *req_path, const char *req_body, unsigned int id = 0) :
    type(req_type),
    identifier(id) {
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

  unsigned int getID() {
    return identifier;
  }
};

class Response {
private:
  int status_code;
  char body[REQUEST_BODY_LEN_MAX]{};
  unsigned int identifier{};

public:
  Response(int resp_code, const char *resp_body, unsigned int id = 0) :
    status_code(resp_code),
    identifier(id) {
    strcpy(body, resp_body);
  }

  int getStatus() {
    return status_code;
  }

  const char *getBody() {
    return &body[0];
  }

  unsigned int getID() {
    return identifier;
  }
};

class Request_Gadget {
protected:
  bool request_gadget_is_ready;

  QueueHandle_t in_request_queue;
  QueueHandle_t in_response_queue;

  QueueHandle_t out_request_queue;
  QueueHandle_t out_response_queue;

  void addIncomingRequest(REQUEST_TYPE new_type, const char *new_path, const char *new_body) {
    auto *buffer_req = new Request(new_type, &new_path[0], &new_body[0]);
    xQueueSend(in_request_queue, &buffer_req, portMAX_DELAY);
  };

  void addIncomingResponse(int new_response_code, const char *new_response_body) {
    auto *buffer_res = new Response(new_response_code, &new_response_body[0]);
    xQueueSend(in_response_queue, &buffer_res, portMAX_DELAY);
  };

  virtual void executeRequestSending(Request *req) = 0;

  virtual void executeResponseSending(Response *res) = 0;

  void sendQueuedItems() {
    if (!request_gadget_is_ready) {
      return;
    }
    if (uxQueueMessagesWaiting(out_request_queue) > 0) {
      Request *buf_req;
      xQueueReceive(out_request_queue, &buf_req, portMAX_DELAY);
      executeRequestSending(buf_req);
      delete buf_req;
    }
    if (uxQueueMessagesWaiting(out_response_queue) > 0) {
      Response *buf_res;
      xQueueReceive(out_response_queue, &buf_res, portMAX_DELAY);
      executeResponseSending(buf_res);
      delete buf_res;
    }
  }

public:
  Request_Gadget() :
    request_gadget_is_ready(false) {
    in_request_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
    in_response_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Response *));
    out_request_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
    out_response_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Response *));
  }

  explicit Request_Gadget(JsonObject data) :
    request_gadget_is_ready(false) {
    in_request_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
    in_response_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Response *));
    out_request_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
    out_response_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Response *));
  }

  bool requestGadgetIsReady() {
    return request_gadget_is_ready;
  }

  bool hasRequest() {
    return uxQueueMessagesWaiting(in_request_queue) > 0;
  }

  Request *getRequest() {
    Request *buf_req;
    xQueueReceive(in_request_queue, &buf_req, portMAX_DELAY);
    return buf_req;
  }

  bool hasResponse() {
    return uxQueueMessagesWaiting(in_response_queue) > 0;
  }

  Response *getResponse() {
    Response *buf_res;
    xQueueReceive(in_response_queue, &buf_res, portMAX_DELAY);
    return buf_res;
  }

  void sendRequest(REQUEST_TYPE req_type, const char *content_type, IPAddress ip, int port, const char *req_path,
                   const char *req_body) {
    auto *buffer_req = new Request(req_type, req_path, req_body);
    xQueueSend(out_request_queue, &buffer_req, portMAX_DELAY);
  };

  void sendRequest(REQUEST_TYPE req_type, const char *content_type, IPAddress ip, int port, const char *req_path,
                   JsonObject req_body) {
    char buffer[REQUEST_BODY_LEN_MAX]{};
    serializeJson(req_body, buffer);
    sendRequest(req_type, content_type, ip, port, req_path, buffer);
  };

  void sendResponse(int status_code, const char *res_body) {
    auto *buffer_res = new Response(status_code, res_body);
    xQueueSend(out_response_queue, &buffer_res, portMAX_DELAY);
  }

  void sendResponse(int status_code, JsonObject res_body) {
    char buffer[REQUEST_BODY_LEN_MAX]{};
    serializeJson(res_body, buffer);
    sendResponse(status_code, &buffer[0]);
  }

  virtual void refresh() = 0;
};

#endif //__Request_Gadget__