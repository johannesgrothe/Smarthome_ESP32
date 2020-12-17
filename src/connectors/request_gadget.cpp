#include "request_gadget.h"

#include <utility>

// Request_Gadget
void Request_Gadget::addIncomingRequest(std::shared_ptr<Request> request) {
  xQueueSend(in_request_queue_, &request, portMAX_DELAY);
};

void Request_Gadget::sendQueuedItems() {
  if (!request_gadget_is_ready_) {
    return;
  }
  if (uxQueueMessagesWaiting(out_request_queue_) > 0) {
      std::shared_ptr<Request> buf_req;
    xQueueReceive(out_request_queue_, &buf_req, portMAX_DELAY);
    executeRequestSending(buf_req);
  }
}

Request_Gadget::Request_Gadget() :
  type_(RequestGadgetType::NONE_G),
  request_gadget_is_ready_(false) {
  in_request_queue_ = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(std::shared_ptr<Request>));
  out_request_queue_ = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(std::shared_ptr<Request>));
}

Request_Gadget::Request_Gadget(RequestGadgetType t) :
  type_(t),
  request_gadget_is_ready_(false) {
  in_request_queue_ = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(std::shared_ptr<Request>));
  out_request_queue_ = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(std::shared_ptr<Request>));
  request_gadget_is_ready_ = true;
}

bool Request_Gadget::requestGadgetIsReady() const {
  return request_gadget_is_ready_;
}

RequestGadgetType Request_Gadget::getGadgetType() {
  return type_;
}

bool Request_Gadget::hasRequest() {
  return uxQueueMessagesWaiting(in_request_queue_) > 0;
}

std::shared_ptr<Request> Request_Gadget::getRequest() {
  std::shared_ptr<Request> buf_req;
  xQueueReceive(in_request_queue_, &buf_req, portMAX_DELAY);
  return buf_req;
}

void Request_Gadget::sendRequest(std::shared_ptr<Request> request) {
  xQueueSend(out_request_queue_, &request, portMAX_DELAY);
}

std::shared_ptr<Request> Request_Gadget::waitForResponse(int id, unsigned long wait_time) {
  {
    unsigned long end_time = millis() + wait_time;

    std::vector<std::shared_ptr<Request>> buffered_requests;

    std::shared_ptr<Request> out_req = nullptr;

    while (millis() < end_time) {

      std::shared_ptr<Request> buf_req;
      xQueueReceive(in_request_queue_, &buf_req, portMAX_DELAY);

      if (buf_req->getID() == id) {
        out_req = buf_req;
        break;
      } else {
        buffered_requests.push_back(buf_req);
      }
    }
    for (auto buf_req: buffered_requests){
      xQueueSend(in_request_queue_, &buf_req, portMAX_DELAY);
    }
    return out_req;
  }
}

std::shared_ptr<Request> Request_Gadget::sendRequestAndWaitForResponse(const std::shared_ptr<Request>& request, unsigned long wait_time) {
  sendRequest(request);
  return waitForResponse(request->getID(), wait_time);
};