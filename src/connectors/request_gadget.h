#ifndef __Request_Gadget__
#define __Request_Gadget__

// External Imports
#include <ArduinoJson.h>
#include <cstring>
#include <utility>
#include <IPAddress.h>
#include <WiFi.h>

// Other Imports
#include "../system_settings.h"
#include "../console_logger.h"

enum GADGET_TYPE {
  MQTT_G, SERIAL_G, NONE_G
};

class Request {
private:
  char body[REQUEST_BODY_LEN_MAX]{};
  char path[REQUEST_PATH_LEN_MAX]{};
  std::function<void(Request *)> send_answer;

protected:

  bool can_respond;
  bool needs_response;

public:
  Request(const char *req_path, const char *req_body) :
    can_respond(false),
    needs_response(false) {
    strncpy(path, req_path, REQUEST_PATH_LEN_MAX);
    strncpy(body, req_body, REQUEST_BODY_LEN_MAX);
  }

  Request(const char *req_path, const char *req_body, std::function<void(Request *request)> answer_method) :
    can_respond(true),
    needs_response(true) {
    strncpy(path, req_path, REQUEST_PATH_LEN_MAX);
    strncpy(body, req_body, REQUEST_BODY_LEN_MAX);
    send_answer = answer_method;
  }

  virtual ~Request() {
    logger.print(LOG_WARN, "Deleting ");
    logger.addln(path);
  };

  const char *getPath() {
    return &path[0];
  }

  const char *getBody() {
    return &body[0];
  }

  bool respond(const char *res_path, const char *res_body) {
    needs_response = false;
    if (!can_respond) {
      return false;
    }
    auto *req = new Request(res_path, res_body);
    send_answer(req);
    return true;
  }

  void dontRespond() {
    needs_response = false;
  }

};

class Request_Gadget {
protected:
  GADGET_TYPE type;

  bool request_gadget_is_ready;

  QueueHandle_t in_request_queue;

  QueueHandle_t out_request_queue;

  void addIncomingRequest(Request *request) {
    xQueueSend(in_request_queue, &request, portMAX_DELAY);
  };

  virtual void executeRequestSending(Request *req) = 0;

  void sendQueuedItems() {
    if (!request_gadget_is_ready) {
      return;
    }
    if (uxQueueMessagesWaiting(out_request_queue) > 0) {
      Request *buf_req;
      xQueueReceive(out_request_queue, &buf_req, portMAX_DELAY);
      executeRequestSending(buf_req);
//      delete buf_req;  // crashes with LoadProhibited
    }
  }

public:
  Request_Gadget() :
    type(NONE_G),
    request_gadget_is_ready(false) {
    in_request_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
    out_request_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
  }

  explicit Request_Gadget(GADGET_TYPE t, JsonObject data) :
    type(t),
    request_gadget_is_ready(false) {
    in_request_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
    out_request_queue = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
  }

  bool requestGadgetIsReady() {
    return request_gadget_is_ready;
  }

  GADGET_TYPE getGadgetType() {
    return type;
  }

  bool hasRequest() {
    return uxQueueMessagesWaiting(in_request_queue) > 0;
  }

  Request *getRequest() {
    Request *buf_req;
    xQueueReceive(in_request_queue, &buf_req, portMAX_DELAY);
    return buf_req;
  }

  void sendRequest(Request *request) {
    xQueueSend(out_request_queue, &request, portMAX_DELAY);
  };

  virtual void refresh() = 0;
};

#endif //__Request_Gadget__