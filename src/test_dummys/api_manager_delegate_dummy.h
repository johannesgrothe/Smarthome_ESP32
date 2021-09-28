#pragma once

#include "../api/api_manager_delegate.h"

class ApiManagerDelegateDummy : public ApiManagerDelegate {
private:

  std::string id_;

public:

  ApiManagerDelegateDummy(std::string id) :
      ApiManagerDelegate(),
      id_(id) {}

  void handleGadgetUpdate(std::shared_ptr<GadgetMeta> gadget) override {

  }

  void handleCode(std::shared_ptr<CodeCommand> code) override {

  }

  void handleEvent(std::shared_ptr<Event> event) override {

  }

  std::string getClientId() override {
    return id_;
  }

  ClientMeta getClientData() override {
    return {1776,
            {},
            BootMode::Full_Operation,
            "",
            "",
            ""};
  }

  std::vector<GadgetMeta> getGadgetData() override {
    std::vector<GadgetMeta> out_vector = {};
    return out_vector;
  }

};
