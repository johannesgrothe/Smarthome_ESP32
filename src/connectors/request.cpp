#include "request.h"

#include <utility>

Request::Request(std::string req_path, int session_id, std::string sender, std::string receiver, bool is_response,
                 DynamicJsonDocument payload) :
    path_(std::move(req_path)),
    session_id_(session_id),
    sender_(std::move(sender)),
    receiver_(std::move(receiver)),
    is_response_(is_response),
    payload_(std::move(payload)),
    can_respond_(false) {}

Request::Request(std::string req_path, int session_id, std::string sender, bool is_response,
                 DynamicJsonDocument payload) :
    path_(std::move(req_path)),
    session_id_(session_id),
    sender_(std::move(sender)),
    receiver_("null"),
    is_response_(is_response),
    payload_(std::move(payload)),
    can_respond_(false) {}

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
  return respond(ack, "");
}

bool Request::respond(bool ack, const std::string &status_msg) {
  DynamicJsonDocument doc(1000);
  doc["ack"] = ack;
  if (!status_msg.empty()) {
    doc["status_msg"] = status_msg;
  }
  return respond(path_, doc);
}

bool Request::respond(bool ack, const std::string &status_msg, const std::string &path) {
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

bool Request::respond(const std::string &res_path, const DynamicJsonDocument &payload) {
  if (!can_respond_) {
    logger_e("Request", "Failed to respond to request: No response callback set");
    return false;
  }

  // TODO: needed?
//  if (!payload.containsKey("ack")) {
//    payload["ack"] = nullptr;
//  }
//
//  if (!payload.containsKey("status_msg")) {
//    payload["status_msg"] = nullptr;
//  }

  auto new_sender = receiver_;
  auto new_receiver = sender_;
  auto req = std::make_shared<Request>(res_path,
                                       session_id_,
                                       new_sender,
                                       new_receiver,
                                       true,
                                       payload);
  send_answer_(req);
  return true;
}

std::string Request::getBody() const {
  DynamicJsonDocument body_doc(3000);
  body_doc["session_id"] = session_id_;
  body_doc["sender"] = sender_;
  body_doc["receiver"] = receiver_;
  body_doc["is_response"] = is_response_;
  body_doc["payload"] = payload_;

  char buf_arr[3200];
  serializeJson(body_doc, buf_arr, 3200);

  std::stringstream str_buf;
  str_buf << buf_arr;

  std::string out_str = str_buf.str();

  return out_str;
}

bool Request::hasReceiver() const {
  return receiver_ != "null";
}

std::tuple<bool, std::string> Request::getAck() {
  bool ack = false;
  std::string status_msg;
  if (payload_.containsKey("ack")) {
    ack = payload_["ack"].as<bool>();
    if (payload_.containsKey("status_message")) {
      status_msg = payload_["status_message"].as<std::string>();
    }
  }
  return std::tuple<bool, std::string>(ack, status_msg);
}

bool Request::operator==(const Request &rhs) const {
  return getPath() == rhs.getPath() &&
         getID() == rhs.getID() &&
         getSender() == rhs.getSender() &&
         getReceiver() == rhs.getReceiver() &&
         getPayload() == rhs.getPayload();
}

bool Request::operator!=(const Request &rhs) const {
  return !(*this == rhs);
}

void Request::setResponseCallback(std::function<void(std::shared_ptr<Request>)> answer_method) {
  can_respond_ = true;
  send_answer_ = std::move(answer_method);
}

bool Request::getIsResponse() const {
  return is_response_;
}
