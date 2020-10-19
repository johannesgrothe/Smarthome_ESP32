#include "request.h"

#include <utility>

Request::Request(std::string req_path, int session_id, std::string sender, std::string receiver, DynamicJsonDocument payload, bool await_answer):
  path_(std::move(req_path)),
  session_id_(session_id),
  sender_(std::move(sender)),
  receiver_(std::move(receiver)),
  payload_(std::move(payload)),
  needs_response_(false),
  can_respond_(false),
  await_response_(await_answer),
  response_(nullptr) {}

Request::Request(std::string req_path, int session_id, std::string sender, std::string receiver, DynamicJsonDocument payload, std::function<void(Request *request)> answer_method) :
  path_(std::move(req_path)),
  session_id_(session_id),
  sender_(std::move(sender)),
  receiver_(std::move(receiver)),
  payload_(std::move(payload)),
  needs_response_(true),
  can_respond_(true),
  send_answer_(std::move(answer_method)),
  await_response_(false),
  response_(nullptr) {}

Request::~Request() {
  logger.print(LOG_TYPE::WARN, "Deleting ");
  logger.println(path_);
}

std::string Request::getPath() const {
  return path_;
}

int Request::getID() const {
  return session_id_;
}

std::string Request::getSender() const {
  return sender_;
}

std::string Request::getReceiver() const {
  return receiver_;
}

void Request::updateReceiver(std::string receiver) {
  receiver_ = std::move(receiver);
}

DynamicJsonDocument Request::getPayload() const {
  return payload_;
}

bool Request::respond(bool ack) {
  return respond(ack, path_);
}

bool Request::respond(bool ack, const std::string& status_msg) {
  DynamicJsonDocument doc(1000);
  doc["ack"] = ack;
  if (!status_msg.empty()) {
    doc["status_msg"] = status_msg;
  }
  return respond(path_, doc);
}

bool Request::respond(bool ack, const std::string& status_msg, const std::string& path) {
  DynamicJsonDocument doc(1000);
  doc["ack"] = ack;
  if (!status_msg.empty()) {
    doc["status_msg"] = status_msg;
  }
  return respond(path, doc);
}

bool Request::respond(const DynamicJsonDocument &payload) {
  return respond(path_, payload);
}

bool Request::respond(const std::string& res_path, const DynamicJsonDocument& payload) {
  needs_response_ = false;
  if (!can_respond_) {
    return false;
  }
  auto new_sender = receiver_;
  auto new_receiver = sender_;
  auto *req = new Request(res_path,
                          session_id_,
                          new_sender,
                          new_receiver,
                          payload);
  send_answer_(req);
  return true;
}

void Request::dontRespond() {
  needs_response_ = false;
}

std::string Request::getBody() const {
  std::stringstream out_str;
  char bufchrarr[1022];
  serializeJson(payload_, bufchrarr);
  out_str << R"({"session_id": )" << session_id_
          << R"(, "sender": ")" << sender_
          << R"(", "receiver": ")" << receiver_
          << R"(", "payload": )" << bufchrarr << "}";
  return out_str.str();
}

bool Request::hasReceiver() const {
  return receiver_ != "null";
}

bool Request::hasResponse() {
  return response_ != nullptr;
}

std::shared_ptr<Request> Request::getResponse() {
  return response_;
}

void Request::setResponse(std::shared_ptr<Request> res) {
  response_ = std::move(res);
}
