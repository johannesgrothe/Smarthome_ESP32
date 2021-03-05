#include "request_gadget.h"

#include <utility>

QueueHandle_t createRequestQueue() {
  return xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
}

// Request_Gadget
void Request_Gadget::addIncomingRequest(Request *request) {
  xQueueSend(buffer_in_request_queue_, &request, portMAX_DELAY);
};

void Request_Gadget::sendQueuedItems() {
  if (!request_gadget_is_ready_) {
    return;
  }
  if (uxQueueMessagesWaiting(out_request_queue_) > 0) {
    Request *buf_req;
    xQueueReceive(out_request_queue_, &buf_req, portMAX_DELAY);
    executeRequestSending(buf_req);
    delete buf_req;
  }
}

Request_Gadget::Request_Gadget() :
    split_req_buffer(nullptr),
    type_(RequestGadgetType::NONE_G),
    request_gadget_is_ready_(false) {
  buffer_in_request_queue_ = createRequestQueue();
  in_request_queue_ = createRequestQueue();
  out_request_queue_ = createRequestQueue();
}

Request_Gadget::Request_Gadget(RequestGadgetType t) :
    split_req_buffer(nullptr),
    type_(t),
    request_gadget_is_ready_(false) {
  buffer_in_request_queue_ = createRequestQueue();
  in_request_queue_ = createRequestQueue();
  out_request_queue_ = createRequestQueue();
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

Request *Request_Gadget::getRequest() {
  Request *buf_req;
  xQueueReceive(in_request_queue_, &buf_req, portMAX_DELAY);
  return buf_req;
}

void Request_Gadget::sendRequest(Request *request) {
  xQueueSend(out_request_queue_, &request, portMAX_DELAY);
}

Request *Request_Gadget::waitForResponse(int id, unsigned long wait_time) {
  {
    unsigned long end_time = millis() + wait_time;

    std::vector<Request *> buffered_requests;

    Request *out_req = nullptr;

    while (millis() < end_time) {

      Request *buf_req;
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

Request *Request_Gadget::sendRequestAndWaitForResponse(Request *request, unsigned long wait_time) {
  sendRequest(request);
  return waitForResponse(request->getID(), wait_time);
}

void Request_Gadget::refresh() {
  if (!request_gadget_is_ready_) {
    return;
  }
  refresh_network();
  if (uxQueueMessagesWaiting(buffer_in_request_queue_) > 0) {
    Request *buf_req;
    xQueueReceive(buffer_in_request_queue_, &buf_req, portMAX_DELAY);
    auto req_payload = buf_req->getPayload();

    // Check if the request is a normal or split one
    if (req_payload.containsKey("package_index") && req_payload.containsKey("split_payload")) {

      // Request is split, collect parts and submit it to queue after that
      logger.println("Split request found");
      auto p_index = req_payload["package_index"].as<int>();
      auto split_payload = req_payload["split_payload"].as<std::string>();

      logger.println(p_index);
      logger.println(split_payload.c_str());

      // Check if its the first request
      if (p_index == 0) {
        logger.println("First Request");

        if (req_payload.containsKey("last_index")) {
          auto last_index = req_payload["last_index"].as<int>();
          split_req_buffer = std::make_shared<SplitRequestBuffer>(buf_req->getID(),
                                                                  buf_req->getPath(),
                                                                  buf_req->getSender(),
                                                                  buf_req->getReceiver(),
                                                                  last_index - 1
          );
          split_req_buffer->addData(0, split_payload);
        } else {
          split_req_buffer = nullptr;
        }
      } else {
        split_req_buffer->addData(p_index, split_payload);
        auto out_req = split_req_buffer->getRequest();
        if (out_req != nullptr) {
          logger.println("Stitched together the split request");
          xQueueSend(in_request_queue_, &out_req, portMAX_DELAY);
          split_req_buffer = nullptr;
        }
      }
    } else {

      // Request is a normal one, put it in queue to be accessible to the outside
      logger.println("Normal request found");
      xQueueSend(in_request_queue_, &buf_req, portMAX_DELAY);
    }
  }
};