#pragma once

//Normal Imports
#include "ArduinoJson.h"
#include <memory>
#include "../console_logger.h"
#include "storage/system_storage.h"

// Gadget
#include "gadget.h"

//Lamps
#include "lamp_neopixel.h"
#include "lamp_neopixel_basic.h"
#include "lamp_basic.h"
#include "doorbell_basic.h"

//Fan
#include "fan_westinghouse_ir.h"
#include "lamp_westinghouse_ir.h"

//Switch
#include "wallswitch_basic.h"

//Sensors
#include "sensor_motion_HR501.h"
#include "sensor_temperature_DHT.h"

/**
 * Creates a new gadget out of the given information.
 * The returned gadget may not be successfully initialized.
 * Returns nullptr if no initialization is possible
 * @param gadget_type Which type of gadget to create
 * @param pins Pins ready to use for the gadget
 * @param name Name of the gadget
 * @param gadget_config Config information for the gadget
 * @return A shared pointer to the gadget
 */
static std::shared_ptr<Gadget> createGadgetHelper(GadgetIdentifier gadget_type, port_set pins, const std::string& name, JsonObject gadget_config) {
  switch (gadget_type) {
    case GadgetIdentifier::doorbell_basic:
      return createSHDoorbellBasic(name, pins, gadget_config);

    case GadgetIdentifier::lamp_neopixel_basic:
      return createSHLampNeoPixelBasic(name, pins, gadget_config);

    case GadgetIdentifier::lamp_basic:
      return createSHLampBasic(name, pins, gadget_config);

    case GadgetIdentifier::fan_westinghouse_ir:
      return createSHFanWestinghouseIR(name, pins, gadget_config);

    case GadgetIdentifier::lamp_westinghouse_ir:
      return create_LampWestinghouseIR(name, pins, gadget_config);

    case GadgetIdentifier::wallswitch_basic:
      return create_SwitchBasic(name, pins, gadget_config);

    case GadgetIdentifier::sensor_motion_hr501:
      return create_SensorMotionHR501(name, pins, gadget_config);

    case GadgetIdentifier::sensor_temperature_dht:
      return create_SensorTemperatureDHT(name, pins, gadget_config);

    default:
      return nullptr;
  }
}

/**
 * Creates a new gadget out of the given information.
 * The returned gadget is completely initialized and ready to go.
 * Returns nullptr if no initialization is possible
 * @param gadget_type Which type of gadget to create
 * @param pins Pins ready to use for the gadget
 * @param name Name of the gadget
 * @param gadget_config Config information for the gadget
 * @return A shared pointer to the gadget
 */
static std::shared_ptr<Gadget> createGadget(GadgetIdentifier gadget_type, port_set pins, const std::string& name, JsonObject gadget_config) {
  auto buf_gadget = createGadgetHelper(gadget_type, pins, name, gadget_config);
  if (buf_gadget != nullptr && !buf_gadget->hasInitError()) {
    return buf_gadget;
  }
  logger_e("GadgetFactory", "Gadget could not be successfully initialized and was discarded");
  return nullptr;
}

/**
 * Function that returns whether the gadget requires IR access
 * @param gadget Gadget type to check
 * @return Whether the stated gadget type needs ir access
 */
static bool gadgetRequiresIR(GadgetIdentifier gadget) {
  switch (gadget) {
    case GadgetIdentifier::fan_westinghouse_ir:
    case GadgetIdentifier::lamp_westinghouse_ir:
      return true;
    default:
      return false;
  }
}

/**
 * Function that returns whether the gadget requires radio access
 * @param gadget Gadget type to check
 * @return Whether the stated gadget type needs radio access
 */
static bool gadgetRequiresRadio(GadgetIdentifier gadget) {
  switch (gadget) {
//    case GadgetIdentifier::lamp_westinghouse_ir:
//      return true;
    default:
      return false;
  }
}