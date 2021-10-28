#pragma once


// External Imports
#include <ArduinoJson.h>
#include <cstring>
#include <utility>

// Other Imports
#include "../system_settings.h"
#include "../console_logger.h"
#include "../hardware_controller.h"
#include "request.h"
#include "split_request_buffer.h"
#include "request_queue.h"

enum class RequestGadgetType {
  MQTT_G, SERIAL_G, NONE_G
};

class RequestGadget {
private:
  // Buffer to store split request info until all parts are received
  std::shared_ptr<SplitRequestBuffer> split_req_buffer_;

  void addRequestToInQueue(const std::shared_ptr<Request>& req);

protected:
  // Type of the Gadget
  RequestGadgetType type_;

  // Whether all initialization was successful
  bool request_gadget_is_ready_;

  // Queue for requests detected by the implementation in child class
  RequestQueue buffer_in_request_queue_;

  // Queue for new requests ready to be accessible by the outside
  RequestQueue in_request_queue_;

  // Queue for requests that need to be send
  RequestQueue out_request_queue_;

  /**
   * Adds a request to the 'incoming'-queue
   * @param request Request to be added
   */
  void addIncomingRequest(const std::shared_ptr<Request>& request);

  /**
   * Sends a request to the network
   * @param request Request to be sent
   */
  virtual void executeRequestSending(std::shared_ptr<Request> request) = 0;

  /**
   * Sends requests queued in the out-queue
   */
  void sendQueuedItems();

  /**
   * Method for the gadget to implement sending and receiving requests from their hardware
   */
  virtual void refresh_network() = 0;

public:
  /**
   * Default constructor for the request gadget
   */
  RequestGadget();

  /**
   * Creates a request gadget with a specific type
   * @param t Type of the gadget (self)
   */
  explicit RequestGadget(RequestGadgetType t);

  /**
   * @return Whether te gadget is ready to send and receive requests
   */
  bool requestGadgetIsReady() const;

  /**
   * @return The gadget type of this gadget
   */
  RequestGadgetType getGadgetType();

  /**
   * @return Whether the gadget has received a new request
   */
  bool hasRequest();

  /**
   * Gets the oldest request the gadget has received.
   * Returns nullptr if there is none.
   * @return A request if there is any
   */
  std::shared_ptr<Request> getRequest();

  /**
   * Stores a request to be sent and sends it as soon as possible
   * @param request The request to be sent
   */
  void sendRequest(const std::shared_ptr<Request>& request);

  /**
   * Sends a request and waits for a response to arrive.
   * @param request Request to be sent
   * @param wait_time The time in ms to wait before returning nullptr
   * @return A pointer to the response or a nullptr
   */
  std::shared_ptr<Request> sendRequestAndWaitForResponse(const std::shared_ptr<Request>& request, unsigned long wait_time);

  /**
   * Waits for the Request with the given ID to
   * @param id The id of the response
   * @param wait_time The time in ms to wait before returning nullptr
   * @return A pointer to the response or a nullptr
   */
  std::shared_ptr<Request> waitForResponse(int id, unsigned long wait_time);

  /**
   * Loop-function for the request gadget
   */
  void refresh();
};
