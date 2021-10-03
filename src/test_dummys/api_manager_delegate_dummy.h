#pragma once

#include <utility>

#include "../api/api_manager_delegate.h"

class ApiManagerDelegateDummy : public ApiManagerDelegate {
private:

  std::string id_;

public:

  bool had_gadget_meta;

  explicit ApiManagerDelegateDummy(std::string id) :
      ApiManagerDelegate(),
      id_(std::move(id)),
      had_gadget_meta(false) {}

  void handleGadgetUpdate(GadgetMeta gadget) override {
    had_gadget_meta = true;
  }

  void handleCode(CodeCommand code) override {

  }

  void handleEvent(Event event) override {

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
