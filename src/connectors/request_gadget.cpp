#include "request_gadget.h"

static const char *TAG = "RequestGadget";

// RequestGadget
void RequestGadget::addIncomingRequest(std::shared_ptr<Request> request) {
  buffer_in_request_queue_.push(request);
}

void RequestGadget::sendQueuedItems() {
  if (!request_gadget_is_ready_) {
    return;
  }
  auto buf_req = out_request_queue_.pop();
  if (buf_req) {
    executeRequestSending(buf_req);
  }
}

RequestGadget::RequestGadget() :
    split_req_buffer_(nullptr),
    type_(RequestGadgetType::NONE_G),
    request_gadget_is_ready_(false),
    buffer_in_request_queue_(),
    in_request_queue_(),
    out_request_queue_() {}

RequestGadget::RequestGadget(RequestGadgetType t) :
    split_req_buffer_(nullptr),
    type_(t),
    request_gadget_is_ready_(false),
    buffer_in_request_queue_(),
    in_request_queue_(),
    out_request_queue_() {}

bool RequestGadget::requestGadgetIsReady() const {
  return request_gadget_is_ready_;
}

RequestGadgetType RequestGadget::getGadgetType() {
  return type_;
}

bool RequestGadget::hasRequest() {
  return !in_request_queue_.isEmpty();
}

std::shared_ptr<Request> RequestGadget::getRequest() {
  return in_request_queue_.pop();
}

void RequestGadget::sendRequest(std::shared_ptr<Request> request) {
  out_request_queue_.push(request);
}

std::shared_ptr<Request> RequestGadget::waitForResponse(int id, unsigned long wait_time) {
  {
    unsigned long end_time = millis() + wait_time;

    std::vector<std::shared_ptr<Request>> buffered_requests;

    std::shared_ptr<Request> out_req = nullptr;

    while (millis() < end_time) {

      auto buf_req = in_request_queue_.pop();

      if (buf_req->getID() == id) {
        out_req = buf_req;
        break;
      } else {
        buffered_requests.push_back(buf_req);
      }
    }
    for (auto buf_req: buffered_requests) {
      in_request_queue_.push(buf_req);
    }
    return out_req;
  }
}

std::shared_ptr<Request>
RequestGadget::sendRequestAndWaitForResponse(std::shared_ptr<Request> request, unsigned long wait_time) {
  sendRequest(request);
  return waitForResponse(request->getID(), wait_time);
}

void RequestGadget::refresh() {
  if (!request_gadget_is_ready_) {
    return;
  }
  refresh_network();
  if (!buffer_in_request_queue_.isEmpty()) {
    auto buf_req = buffer_in_request_queue_.pop();
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
          in_request_queue_.push(out_req);
          split_req_buffer_ = nullptr;
        }
      }
    } else {

      // Request is a normal one, put it in queue to be accessible to the outside
      in_request_queue_.push(buf_req);
    }
  }
}
