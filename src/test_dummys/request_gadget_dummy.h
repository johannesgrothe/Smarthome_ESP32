#include "../connectors/request_gadget.h"

class RequestGadgetDummy : public RequestGadget {
private:

  std::shared_ptr<Request> last_send_;

public:
  RequestGadgetDummy() :
      RequestGadget(),
      last_send_(nullptr) {};

  void refresh_network() override {

  }

  void executeRequestSending(std::shared_ptr<Request> request) override {
    last_send_ = request;
  }

  std::shared_ptr<Request> getLastSentRequest() {
    return last_send_;
  }
};
