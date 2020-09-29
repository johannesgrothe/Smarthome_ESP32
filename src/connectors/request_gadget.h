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
#include "request.h"

enum class RequestGadgetType {
  MQTT_G, SERIAL_G, NONE_G
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
