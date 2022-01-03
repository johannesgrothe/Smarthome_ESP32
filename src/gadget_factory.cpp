#include "gadget_factory.h"

#include <utility>

GadgetFactory::GadgetFactory(std::shared_ptr<IR_Gadget> ir, std::shared_ptr<Radio_Gadget> radio) :
    ir_(std::move(ir)),
    radio_(std::move(radio)) {}

std::shared_ptr<Gadget> GadgetFactory::createGadgetHelper(gadget_definitions::GadgetIdentifier gadget_type,
                                                          port_set pins,
                                                          const std::string &name,
                                                          JsonObject gadget_config) {
  switch (gadget_type) {
    case gadget_definitions::GadgetIdentifier::doorbell_basic:
      return createSHDoorbellBasic(name, pins, gadget_config);

      #ifndef UNIT_TEST
      case gadget_definitions::GadgetIdentifier::lamp_neopixel_rgb_basic:
        return createSHLampNeoPixelBasic(name, pins, gadget_config);
      #endif

    case gadget_definitions::GadgetIdentifier::lamp_basic:
      return createSHLampBasic(name, pins, gadget_config);

    case gadget_definitions::GadgetIdentifier::fan_westinghouse_ir:
      return createSHFanWestinghouseIR(name, pins, gadget_config);

    case gadget_definitions::GadgetIdentifier::lamp_westinghouse_ir:
      return create_LampWestinghouseIR(name, pins, gadget_config);

    case gadget_definitions::GadgetIdentifier::wallswitch_basic:
      return create_SwitchBasic(name, pins, gadget_config);

      #ifndef UNIT_TEST
      case gadget_definitions::GadgetIdentifier::sensor_motion_hr501:
        return create_SensorMotionHR501(name, pins, gadget_config);

      case gadget_definitions::GadgetIdentifier::sensor_temperature_dht:
        return create_SensorTemperatureDHT(name, pins, gadget_config);
      #endif

    default:
      return nullptr;
  }
}

std::shared_ptr<Gadget> GadgetFactory::createGadget(gadget_definitions::GadgetIdentifier gadget_type,
                                                    port_set pins,
                                                    const std::string &name,
                                                    JsonObject gadget_config,
                                                    std::vector<gadget_event_map> event_mapping) {
  auto buf_gadget = createGadgetHelper(gadget_type, pins, name, gadget_config);
  if (buf_gadget == nullptr || buf_gadget->hasInitError()) {
    logger_e("GadgetFactory", "Gadget could not be successfully initialized and was discarded");
    return nullptr;
  }

  // IR
  if (GadgetFactory::gadgetRequiresIR(gadget_type)) {
    if (ir_ != nullptr) {
      logger_i("System", "Linking IR gadget");
      buf_gadget->setIR(ir_);
    } else {
      logger_e("System", "No IR gadget available");
      return nullptr;
    }
  } else {
    logger_i("System", "No IR required");
  }

  // Radio
  if (GadgetFactory::gadgetRequiresRadio(gadget_type)) {
    if (radio_ != nullptr) {
      logger_i("System", "Linking radio gadget");
      buf_gadget->setRadio(radio_);
    } else {
      logger_i("System", "No radio gadget available");
      return nullptr;
    }
  } else {
    logger_i("System", "No radio required");
  }

  buf_gadget->setMapping(std::move(event_mapping));

  return buf_gadget;
}

bool GadgetFactory::gadgetRequiresIR(gadget_definitions::GadgetIdentifier gadget) {
  if (std::any_of(IR_GADGETS.begin(),
                  IR_GADGETS.end(),
                  [gadget](gadget_definitions::GadgetIdentifier ident) { return ident == gadget; }))
    return true;
  return false;
}

bool GadgetFactory::gadgetRequiresRadio(gadget_definitions::GadgetIdentifier gadget) {
  if (std::any_of(RADIO_GADGETS.begin(),
                  RADIO_GADGETS.end(),
                  [gadget](gadget_definitions::GadgetIdentifier ident) { return ident == gadget; }))
    return true;
  return false;
}
