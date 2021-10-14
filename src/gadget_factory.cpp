#include "gadget_factory.h"

std::shared_ptr<SH_Gadget>
GadgetFactory::createGadgetHelper(GadgetIdentifier gadget_type, port_set pins, const std::string &name,
                                  JsonObject gadget_config) {
  switch (gadget_type) {
    case GadgetIdentifier::doorbell_basic:
      return createSHDoorbellBasic(name, pins, gadget_config);

      #ifndef UNIT_TEST
      case GadgetIdentifier::lamp_neopixel_basic:
        return createSHLampNeoPixelBasic(name, pins, gadget_config);
      #endif

    case GadgetIdentifier::lamp_basic:
      return createSHLampBasic(name, pins, gadget_config);

    case GadgetIdentifier::fan_westinghouse_ir:
      return createSHFanWestinghouseIR(name, pins, gadget_config);

    case GadgetIdentifier::lamp_westinghouse_ir:
      return createSHLampWestinghouseIR(name, pins, gadget_config);

    case GadgetIdentifier::wallswitch_basic:
      return createSHWallswitchBasic(name, pins, gadget_config);

      #ifndef UNIT_TEST
      case GadgetIdentifier::sensor_motion_hr501:
        return createSHSensorMotionHR501(name, pins, gadget_config);

      case GadgetIdentifier::sensor_temperature_dht:
        return createSHSensorTemperatureDHT(name, pins, gadget_config);
      #endif

    default:
      return nullptr;
  }
}

std::shared_ptr<SH_Gadget>
GadgetFactory::createGadget(GadgetIdentifier gadget_type, port_set pins, const std::string &name,
                            JsonObject gadget_config) {
  auto buf_gadget = createGadgetHelper(gadget_type, pins, name, gadget_config);
  if (buf_gadget != nullptr && !buf_gadget->hasInitError()) {
    return buf_gadget;
  }
  logger_e("GadgetFactory", "Gadget could not be successfully initialized and was discarded");
  return nullptr;
}

bool GadgetFactory::gadgetRequiresIR(GadgetIdentifier gadget) {
  if (std::any_of(IR_GADGETS.begin(),
                  IR_GADGETS.end(),
                  [gadget](GadgetIdentifier ident) { return ident == gadget; }))
    return true;
  return false;
}

bool GadgetFactory::gadgetRequiresRadio(GadgetIdentifier gadget) {
  if (std::any_of(RADIO_GADGETS.begin(),
                  RADIO_GADGETS.end(),
                  [gadget](GadgetIdentifier ident) { return ident == gadget; }))
    return true;
  return false;
}
