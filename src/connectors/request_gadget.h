#pragma once

// External Imports
#include <ArduinoJson.h>
#include <cstring>
#include <utility>
#include <IPAddress.h>
#include <WiFi.h>

// Other Imports
#include "../system_settings.h"
#include "../console_logger.h"

enum class RequestGadgetType {
  MQTT_G, SERIAL_G, NONE_G
};

class Request {
private:
  char body[REQUEST_BODY_LEN_MAX]{};
  char path[REQUEST_PATH_LEN_MAX]{};
  std::function<void(Request *)> send_answer_;

protected:

  bool can_respond_;
  bool needs_response_;

public:
  Request(const char *req_path, const char *req_body);

  Request(const char *req_path, const char *req_body, std::function<void(Request *request)> answer_method);

  virtual ~Request();

  const char *getPath();

  const char *getBody();

  bool respond(const char *res_path, const char *res_body);

  void dontRespond();

};

class Request_Gadget {
protected:
  RequestGadgetType type_;

  bool request_gadget_is_ready_;

  QueueHandle_t in_request_queue_;

  QueueHandle_t out_request_queue_;

  void addIncomingRequest(Request *request);

  virtual void executeRequestSending(Request *req) = 0;

  void sendQueuedItems();

public:
  Request_Gadget();

  explicit Request_Gadget(RequestGadgetType t, JsonObject data);

  bool requestGadgetIsReady();

  RequestGadgetType getGadgetType();

  bool hasRequest();

  Request *getRequest();

  void sendRequest(Request *request);

  virtual void refresh() = 0;
};
