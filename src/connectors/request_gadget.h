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
  std::string body_;
  std::string path_;
  std::function<void(Request *)> send_answer_;

protected:

  bool can_respond_;
  bool needs_response_;

public:
  Request(std::string req_path, std::string req_body);

  Request(std::string req_path, std::string req_body, std::function<void(Request *request)> answer_method);

  virtual ~Request();

  std::string getPath();

  std::string getBody();

//  bool respond(const char *res_path, const char *res_body);

  bool respond(std::string res_path, std::string res_body);

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

  bool requestGadgetIsReady() const;

  RequestGadgetType getGadgetType();

  bool hasRequest();

  Request *getRequest();

  void sendRequest(Request *request);

  virtual void refresh() = 0;
};
