#pragma once

#include <utility>

#include "../api/gadget_update_meta.h"
#include "../api/api_manager_delegate.h"

class ApiManagerDelegateDummy : public ApiManagerDelegate {
private:

public:

  bool had_gadget_meta;

  explicit ApiManagerDelegateDummy() :
      ApiManagerDelegate(),
      had_gadget_meta(false) {}

  void handleGadgetUpdate(GadgetUpdateMeta gadget) override {
    had_gadget_meta = true;
  }

  void handleEvent(Event event) override {

  }

  ClientMeta getClientData() override {
    return {{},
            BootMode::Full_Operation,
            "",
            "",
            ""};
  }

  std::vector<GadgetMeta> getGadgetData() override {
    std::vector<GadgetMeta> out_vector = {};
    return out_vector;
  }

  bool handleSystemConfigWrite(SystemConfig cfg) override {
    return true;
  }

  bool handleGadgetConfigWrite(GadgetConfig cfg) override {
    return true;
  }

  bool handleEventConfigWrite(EventConfig cfg) override {
    return true;
  }

};
