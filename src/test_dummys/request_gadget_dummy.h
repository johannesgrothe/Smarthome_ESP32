#include "../connectors/request_gadget.h"

class RequestGadgetDummy : public RequestGadget {
private:

public:
  RequestGadgetDummy() :
      RequestGadget() {};

  void refresh_network() override {}

  void executeRequestSending(std::shared_ptr<Request> request) override {}

  std::shared_ptr<Request> getLastSentRequest() {
    if (out_request_queue_.isEmpty()) {
      return nullptr;
    }
    return out_request_queue_.pop();
  }
};
