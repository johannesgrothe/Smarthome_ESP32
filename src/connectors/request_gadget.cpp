#include "request_gadget.h"

#include <utility>

QueueHandle_t createRequestQueue() {
  return xQueueCreate(REQUEST_QUEUE_LEN, sizeof(std::shared_ptr<Request>));
}

// RequestGadget
void RequestGadget::addIncomingRequest(std::shared_ptr<Request>request) {
  xQueueSend(buffer_in_request_queue_, &request, portMAX_DELAY);
}

void RequestGadget::sendQueuedItems() {
  if (!request_gadget_is_ready_) {
    return;
  }
  if (uxQueueMessagesWaiting(out_request_queue_) > 0) {
    Serial.println("b");
    std::shared_ptr<Request>buf_req;
    Serial.println("c");
    xQueueReceive(out_request_queue_, &buf_req, portMAX_DELAY);
    Serial.println("d");
    executeRequestSending(buf_req);
    Serial.println("e");
  }
}

RequestGadget::RequestGadget() :
    split_req_buffer_(nullptr),
    type_(RequestGadgetType::NONE_G),
    request_gadget_is_ready_(false) {
  buffer_in_request_queue_ = createRequestQueue();
  in_request_queue_ = createRequestQueue();
  out_request_queue_ = createRequestQueue();
}

RequestGadget::RequestGadget(RequestGadgetType t) :
    split_req_buffer_(nullptr),
    type_(t),
    request_gadget_is_ready_(false) {
  buffer_in_request_queue_ = createRequestQueue();
  in_request_queue_ = createRequestQueue();
  out_request_queue_ = createRequestQueue();
  request_gadget_is_ready_ = true;
}

bool RequestGadget::requestGadgetIsReady() const {
  return request_gadget_is_ready_;
}

RequestGadgetType RequestGadget::getGadgetType() {
  return type_;
}

bool RequestGadget::hasRequest() {
  return uxQueueMessagesWaiting(in_request_queue_) > 0;
}

std::shared_ptr<Request>RequestGadget::getRequest() {
  std::shared_ptr<Request>buf_req;
  xQueueReceive(in_request_queue_, &buf_req, portMAX_DELAY);
  return buf_req;
}

void RequestGadget::sendRequest(std::shared_ptr<Request>request) {
  xQueueSend(out_request_queue_, &request, portMAX_DELAY);
}

std::shared_ptr<Request>RequestGadget::waitForResponse(int id, unsigned long wait_time) {
  {
    unsigned long end_time = millis() + wait_time;

    std::vector<std::shared_ptr<Request>> buffered_requests;

    std::shared_ptr<Request>out_req = nullptr;

    while (millis() < end_time) {

      std::shared_ptr<Request>buf_req;
      xQueueReceive(in_request_queue_, &buf_req, portMAX_DELAY);

      if (buf_req->getID() == id) {
        out_req = buf_req;
        break;
      } else {
        buffered_requests.push_back(buf_req);
      }
    }
    for (auto buf_req: buffered_requests) {
      xQueueSend(in_request_queue_, &buf_req, portMAX_DELAY);
    }
    return out_req;
  }
}

std::shared_ptr<Request>RequestGadget::sendRequestAndWaitForResponse(std::shared_ptr<Request>request, unsigned long wait_time) {
  sendRequest(request);
  return waitForResponse(request->getID(), wait_time);
}

void RequestGadget::refresh() {
  if (!request_gadget_is_ready_) {
    return;
  }
  refresh_network();
  if (uxQueueMessagesWaiting(buffer_in_request_queue_) > 0) {
    std::shared_ptr<Request>buf_req;
    xQueueReceive(buffer_in_request_queue_, &buf_req, portMAX_DELAY);
    auto req_payload = buf_req->getPayload();

    // Check if the request is a normal or split one
    if (req_payload.containsKey("package_index") && req_payload.containsKey("split_payload")) {

      // Request is split, collect parts and submit it to queue after that
      auto p_index = req_payload["package_index"].as<int>();
      auto split_payload = req_payload["split_payload"].as<std::string>();

      // Check if its the first request
      if (p_index == 0) {

        if (req_payload.containsKey("last_index")) {
          auto last_index = req_payload["last_index"].as<int>();
          split_req_buffer_ = std::make_shared<SplitRequestBuffer>(buf_req->getID(),
                                                                   buf_req->getPath(),
                                                                   buf_req->getSender(),
                                                                   buf_req->getReceiver(),
                                                                   last_index
          );
          split_req_buffer_->addData(0, split_payload);
        } else {
          split_req_buffer_ = nullptr;
        }
      } else {
        split_req_buffer_->addData(p_index, split_payload);
        auto out_req = split_req_buffer_->getRequest();
        if (out_req != nullptr) {
          xQueueSend(in_request_queue_, &out_req, portMAX_DELAY);
          split_req_buffer_ = nullptr;
        }
      }
    } else {

      // Request is a normal one, put it in queue to be accessible to the outside
      xQueueSend(in_request_queue_, &buf_req, portMAX_DELAY);
    }
  }
}
