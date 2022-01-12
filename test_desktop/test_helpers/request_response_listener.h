#pragma once

#include "../../src/connectors/request.h"

class RequestResponseListener {
private:

  std::shared_ptr <Request> out_req_;
  std::shared_ptr <Request> in_req_;

public:
  RequestResponseListener(const std::string& path,
                          const std::string& sender,
                          const std::string& receiver,
                          const DynamicJsonDocument& payload) :
      out_req_(nullptr),
      in_req_(nullptr) {
    out_req_ = std::make_shared<Request>(path, 177787, sender, receiver, payload);
    using std::placeholders::_1;
    out_req_->setResponseCallback(std::bind(&RequestResponseListener::callback, this, _1));
  }

  RequestResponseListener(const std::string& path,
                          const std::string& sender,
                          const DynamicJsonDocument& payload) :
      out_req_(nullptr),
      in_req_(nullptr) {
    out_req_ = std::make_shared<Request>(path, 177787, sender, payload);
    using std::placeholders::_1;
    out_req_->setResponseCallback(std::bind(&RequestResponseListener::callback, this, _1));
  }

  void reset() {
    in_req_ = nullptr;
  }

  std::shared_ptr <Request> getRequest() {
    return out_req_;
  }

  std::shared_ptr <Request> getResponse() {
    return in_req_;
  }

  void callback(std::shared_ptr <Request> request) {
    in_req_ = std::move(request);
  }

};