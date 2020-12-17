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

  void addIncomingRequest(std::shared_ptr<Request>);

  virtual void executeRequestSending(std::shared_ptr<Request>) = 0;

  void sendQueuedItems();

public:
  Request_Gadget();

  explicit Request_Gadget(RequestGadgetType t);

  bool requestGadgetIsReady() const;

  RequestGadgetType getGadgetType();

  bool hasRequest();

  std::shared_ptr<Request> getRequest();

  void sendRequest(std::shared_ptr<Request> request);

  std::shared_ptr<Request> sendRequestAndWaitForResponse(const std::shared_ptr<Request>& request, unsigned long wait_time);

  /**
   * Waits for the Request with the given ID to
   * @param id The id of the response
   * @param wait_time The time in ms to wait before returning nullptr
   * @return A pointer to the response or a nullptr
   */
  std::shared_ptr<Request> waitForResponse(int id, unsigned long wait_time);

  virtual void refresh() = 0;
};
