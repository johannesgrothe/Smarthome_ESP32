#pragma once

#include "../api_manager_delegate.h"

class ApiManagerDelegateDummy : public ApiManagerDelegate {
private:

  std::string id_;

public:

  ApiManagerDelegateDummy(std::string id) :
      ApiManagerDelegate(),
      id_(id) {}

  void handleGadgetUpdate(std::shared_ptr <GadgetMeta> gadget) override {

  }

  void handleCode(std::shared_ptr <CodeCommand> code) override {

  }

  void handleEvent(std::shared_ptr <Event> event) override {

  }

  std::string getClientId() override {
    return id_;
  }

};
