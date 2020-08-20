#include "request_gadget.h"

//Request
Request::Request(const char *req_path, const char *req_body) :
  can_respond_(false),
  needs_response_(false) {
  strncpy(path, req_path, REQUEST_PATH_LEN_MAX);
  strncpy(body, req_body, REQUEST_BODY_LEN_MAX);
}

Request::Request(const char *req_path, const char *req_body, std::function<void(Request *request)> answer_method) :
  can_respond_(true),
  needs_response_(true) {
  strncpy(path, req_path, REQUEST_PATH_LEN_MAX);
  strncpy(body, req_body, REQUEST_BODY_LEN_MAX);
  send_answer_ = answer_method;
}

Request::~Request() {
  logger.print(LOG_TYPE::WARN, "Deleting ");
  logger.addln(path);
}

const char *Request::getPath() {
  return &path[0];
}

const char *Request::getBody() {
  return &body[0];
}

bool Request::respond(const char *res_path, const char *res_body) {
  needs_response_ = false;
  if (!can_respond_) {
    return false;
  }
  auto *req = new Request(res_path, res_body);
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
//      delete buf_req;  // crashes with LoadProhibited
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

bool Request_Gadget::requestGadgetIsReady() {
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