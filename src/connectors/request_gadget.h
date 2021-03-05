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
#include "split_request_buffer.h"

enum class RequestGadgetType {
  MQTT_G, SERIAL_G, NONE_G
};

class Request_Gadget {
private:
  std::shared_ptr<SplitRequestBuffer> split_req_buffer;

protected:

  // Type of the Gadget
  RequestGadgetType type_;

  // Whether all initialization was successful
  bool request_gadget_is_ready_;

  // Queue for requests detected by the implementation in child class
  QueueHandle_t buffer_in_request_queue_;

  // Queue for new requests ready to be accessible by the outside
  QueueHandle_t in_request_queue_;

  // Queue for requests that need to be send
  QueueHandle_t out_request_queue_;

  void addIncomingRequest(Request *);

  virtual void executeRequestSending(Request *) = 0;

  void sendQueuedItems();

public:
  Request_Gadget();

  explicit Request_Gadget(RequestGadgetType t);

  bool requestGadgetIsReady() const;

  RequestGadgetType getGadgetType();

  bool hasRequest();

  Request * getRequest();

  void sendRequest(Request * request);

  Request * sendRequestAndWaitForResponse(Request * request, unsigned long wait_time);

  /**
   * Waits for the Request with the given ID to
   * @param id The id of the response
   * @param wait_time The time in ms to wait before returning nullptr
   * @return A pointer to the response or a nullptr
   */
  Request * waitForResponse(int id, unsigned long wait_time);

  void refresh();

  virtual void refresh_network() = 0;
};
