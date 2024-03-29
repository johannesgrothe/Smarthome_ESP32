#include "gadget.h"

#include <utility>
#include <vector>

Gadget::Gadget::Gadget(std::string name, GadgetType type, std::vector<Characteristic> characteristics) :
    init_error(false),
    name(std::move(name)),
    has_changed(true),
    characteristics_(std::move(characteristics)),
    type(type) {}

GadgetType Gadget::getType() {
  return type;
}

std::string Gadget::getName() const {
  return name;
}

void Gadget::updateInitStatus(bool status_update) {
  if (!status_update) {
    init_error = true;
  }
}

bool Gadget::hasInitError() const {
  return init_error;
}

bool Gadget::hasChanged() {
  bool buf = has_changed;
  has_changed = false;
  if (buf) {
    logger_i(getName(), "Change detected.");
  }
  return buf;
}

//IR Connector

bool Gadget::sendRawIR(const uint16_t raw_data[], const uint8_t content_length) {
  if (hasIR())
    return ir_gadget_->sendRawIR(raw_data, content_length);
  logger_e(getName(), "Cannot send IR: not initialized");
  return false;
}

bool Gadget::sendIR(const unsigned long command, const uint8_t com_type) {
  if (hasIR())
    return ir_gadget_->sendIR(command, com_type);
  logger_e(getName(), "Cannot send IR: not initialized");
  return false;
}

void Gadget::setIR(const std::shared_ptr<IR_Gadget> &new_ir_gadget) {
  ir_gadget_ = new_ir_gadget;
}

bool Gadget::hasIR() const {
  return ir_gadget_ != nullptr;
}

// Radio Connector

void Gadget::setRadio(const std::shared_ptr<Radio_Gadget> &new_radio_gadget) {
  radio_gadget_ = new_radio_gadget;
}

bool Gadget::hasRadio() const {
  return radio_gadget_ != nullptr;
}

bool Gadget::handleCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier characteristic, uint16_t step_value) {
  auto status = setCharacteristicValue(characteristic, step_value);
  if (status) {
    logger_i(getName(), "Updating Characteristic %d -> %d", int(characteristic), step_value);
    executeCharacteristicUpdate(characteristic, step_value);
  }
  return status;
}

bool Gadget::handleEvent(const std::string &event_id) {
  logger_e(getName(), "Handling event '%s'", event_id.c_str());
  for (gadget_event_map event_data: event_mapping_) {
    auto i_event_id = std::get<0>(event_data);
    if (i_event_id == event_id) {
      for (auto characteristic_tuple: std::get<1>(event_data)) {
        auto i_char = gadget_definitions::CharacteristicIdentifier(std::get<0>(characteristic_tuple));
        uint16_t i_val = std::get<1>(characteristic_tuple);
        handleCharacteristicUpdate(i_char, i_val);
      }
      return true;
    }
  }
  return false;
}

std::vector<gadget_event_map> Gadget::getMapping() const {
  return event_mapping_;
}

void Gadget::setMapping(std::vector<gadget_event_map> event_mapping) {
  event_mapping_ = std::move(event_mapping);
}

std::vector<Characteristic> Gadget::getCharacteristics() {
  return characteristics_;
}

uint16_t Gadget::getCharacteristicValue(gadget_definitions::CharacteristicIdentifier characteristic) {
  for (auto c: characteristics_) {
    if (c.type == characteristic) {
      return c.getStepValue();
    }
  }
  return 0;
}

bool Gadget::setCharacteristicValue(gadget_definitions::CharacteristicIdentifier characteristic, uint16_t step_value) {
  for (auto &c: characteristics_) {
    if (c.type == characteristic) {
      if (c.getStepValue() != step_value) {
        c.setStepValue(step_value);
        has_changed = true;
        return true;
      }
      return false;
    }
  }
  return false;
}

std::shared_ptr<Characteristic> Gadget::getCharacteristic(gadget_definitions::CharacteristicIdentifier identifier) {
  for (auto c: characteristics_) {
    if (c.type == identifier) {
      return std::make_shared<Characteristic>(c);
    }
  }
  return nullptr;
}
