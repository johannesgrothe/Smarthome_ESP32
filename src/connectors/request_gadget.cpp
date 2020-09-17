#include "request_gadget.h"

#include <utility>

Request::Request(std::string req_path, std::string req_body) :
  body_(std::move(req_body)),
  path_(std::move(req_path)),
  can_respond_(false),
  needs_response_(false) {}

Request::Request(std::string req_path, std::string req_body, std::function<void(Request *request)> answer_method) :
  body_(std::move(req_body)),
  path_(std::move(req_path)),
  send_answer_(std::move(answer_method)),
  can_respond_(true),
  needs_response_(true) {}

Request::~Request() {
  logger.print(LOG_TYPE::WARN, "Deleting ");
  logger.println(path_);
}

std::string Request::getPath() {
  return path_;
}

std::string Request::getBody() {
  return body_;
}

bool Request::respond(std::string res_path, std::string res_body) {
  needs_response_ = false;
  if (!can_respond_) {
    return false;
  }
  auto *req = new Request(std::move(res_path), std::move(res_body));
  send_answer_(req);
  return true;
}

void Request::dontRespond() {
  needs_response_ = false;
}

// Request_Gadget
void Request_Gadget::addIncomingRequest(Request *request) {
  xQueueSend(in_request_queue_, &request, portMAX_DELAY);
};

void Request_Gadget::sendQueuedItems() {
  if (!request_gadget_is_ready_) {
    return;
  }
  if (uxQueueMessagesWaiting(out_request_queue_) > 0) {
    Request *buf_req;
    xQueueReceive(out_request_queue_, &buf_req, portMAX_DELAY);
    executeRequestSending(buf_req);
  }
}

Request_Gadget::Request_Gadget() :
  type_(RequestGadgetType::NONE_G),
  request_gadget_is_ready_(false) {
  in_request_queue_ = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
  out_request_queue_ = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
}

Request_Gadget::Request_Gadget(RequestGadgetType t, JsonObject data) :
  type_(t),
  request_gadget_is_ready_(false) {
  if (data.isNull()) {
  }
  in_request_queue_ = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
  out_request_queue_ = xQueueCreate(REQUEST_QUEUE_LEN, sizeof(Request *));
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
};