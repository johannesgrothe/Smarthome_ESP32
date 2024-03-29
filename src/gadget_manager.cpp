#include "api/gadget_update_meta.h"
#include "gadget_manager.h"

static const char *TAG = "GadgetManager";

GadgetManager::GadgetManager() = default;

bool GadgetManager::addGadget(const std::shared_ptr<Gadget> &new_gadget) {
  gadgets_.push_back(new_gadget);
  return true;
}

std::shared_ptr<Gadget> GadgetManager::getGadget(const std::string &gadget_name) {
  for (auto gadget: gadgets_) {
    if (gadget->getName() == gadget_name) {
      return gadget;
    }
  }
  return nullptr;
}

std::shared_ptr<Gadget> GadgetManager::getGadget(uint8_t index) const {
  if (index >= gadgets_.size())
    return nullptr;
  return gadgets_[index];
}

std::shared_ptr<Gadget> GadgetManager::operator[](int index) const {
  if (index >= gadgets_.size() || index < 0)
    return nullptr;
  return getGadget(index);
}

uint8_t GadgetManager::getGadgetCount() const {
  return (uint8_t) gadgets_.size();
}

void GadgetManager::refresh() {
  for (const auto& gadget: gadgets_) {
    gadget->refresh();
  }
}

void GadgetManager::forwardEvent(const std::shared_ptr<Event>& event) {
  if (event == nullptr) {
    return;
  }
  for (const auto& gadget: gadgets_) {
    gadget->handleEvent(event->name);
  }
}

void GadgetManager::forwardUpdate(const GadgetUpdateMeta &data) {
  for (const auto& gadget: gadgets_) {
    if (gadget->getName() == data.id) {
      logger_i(TAG, "Updating status for gadget '%s'", data.id.c_str());
      for (auto characteristic: data.characteristics) {
        gadget->handleCharacteristicUpdate(characteristic.type, characteristic.step_val);
      }
      return;
    }
  }
}
