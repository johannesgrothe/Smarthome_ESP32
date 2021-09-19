#include "../connectors/request_gadget.h"

class RequestGadgetDummy : public RequestGadget {
private:

public:
  RequestGadgetDummy() :
      RequestGadget() {};

  void refresh_network() override {

  }

  void executeRequestSending(std::shared_ptr<Request> request) override {

  }
};
