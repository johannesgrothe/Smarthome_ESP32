#include "gadget.h"

#include <utility>

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

bool Gadget::handleCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) {
  logger_i(getName(), "Updating Characteristic %d -> %d", int(characteristic), step_value);
  auto status = setCharacteristicValue(characteristic, step_value);
  if (status) {
    executeCharacteristicUpdate(characteristic, step_value);
  }
  return status;
}

bool Gadget::handleEvent(const std::string &event_id) {
  logger_e(getName(), "Handling event '%s'", event_id.c_str());
  for (mapping_pair event_data: event_mapping_) {
    auto i_event_id = std::get<0>(event_data);
    if (i_event_id == event_id) {
      for (auto characteristic_tuple: std::get<1>(event_data)) {
        auto i_char = CharacteristicIdentifier(std::get<0>(characteristic_tuple));
        uint16_t i_val = std::get<1>(characteristic_tuple);
        handleCharacteristicUpdate(i_char, i_val);
      }
      return true;
    }
  }
  return false;
}

std::vector<mapping_pair> Gadget::getMapping() const {
  return event_mapping_;
}

std::vector<GadgetCharacteristicSettings> Gadget::getCharacteristics() {
  std::vector<GadgetCharacteristicSettings> out_list;
  for (auto c: characteristics_) {
    GadgetCharacteristicSettings data(GadgetCharacteristicSettings(c.type,
                                                                   c.min,
                                                                   c.max,
                                                                   c.steps,
                                                                   c.getStepValue()));
    out_list.push_back(data);
  }
  return out_list;
}

uint16_t Gadget::getCharacteristicValue(CharacteristicIdentifier characteristic) {
  for (auto c: characteristics_) {
    if (c.type == characteristic) {
      return c.getStepValue();
    }
  }
  return 0;
}

bool Gadget::setCharacteristicValue(CharacteristicIdentifier characteristic, uint16_t step_value) {
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

std::shared_ptr<GadgetCharacteristicSettings> Gadget::getCharacteristic(CharacteristicIdentifier identifier) {
  for (auto c: characteristics_) {
    if (c.type == identifier) {
      return std::make_shared<GadgetCharacteristicSettings>(c.type,
                                                            c.min,
                                                            c.max,
                                                            c.steps,
                                                            c.getStepValue());
    }
  }
  return nullptr;
}
