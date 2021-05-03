#include "request.h"

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

Request::Request(std::string req_path, int session_id, std::string sender, std::string receiver, DynamicJsonDocument payload, std::function<void(std::shared_ptr<Request> request)> answer_method) :
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
  auto req = std::make_shared<Request>(res_path,
                                       session_id_,
                                       new_sender,
                                       new_receiver,
                                       payload);
  // TODO: change how responses are handled by returning the response request and not using a callback
  send_answer_(req);
  return true;
}

void Request::dontRespond() {
  needs_response_ = false;
}

std::string Request::getBody() const {
  DynamicJsonDocument body_doc(3000);
  body_doc["session_id"] = session_id_;
  body_doc["sender"] = sender_;
  body_doc["receiver"] = receiver_;
  body_doc["payload"] = payload_;

  char buf_arr[3200];
  auto size = serializeJson(body_doc, buf_arr, 4200);

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
