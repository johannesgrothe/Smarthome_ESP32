#pragma once

//Normal Imports
#include "ArduinoJson.h"
#include <memory>
#include "../console_logger.h"
#include "system_storage.h"

// Gadget
#include "sh_gadget.h"

//Lamps
#include "sh_lamp_neopixel.h"
#include "sh_lamp_neopixel_basic.h"
#include "sh_lamp_basic.h"
#include "sh_doorbell_basic.h"

//Fan
#include "sh_fan_westinghouse_ir.h"
#include "sh_lamp_westinghouse_ir.h"

//Wallswitch
#include "sh_wallswitch_basic.h"

//Sensors
#include "sh_sensor_motion_HR501.h"
#include "sh_sensor_temperature_DHT.h"

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
static std::shared_ptr<SH_Gadget> createGadgetHelper(GadgetIdentifier gadget_type, pin_set pins, const std::string& name, JsonObject gadget_config) {
  switch (gadget_type) {
    case GadgetIdentifier::sh_doorbell_basic:
      return createSHDoorbellBasic(name, pins, gadget_config);

    case GadgetIdentifier::sh_lamp_neopixel_basic:
      return createSHLampNeoPixelBasic(name, pins, gadget_config);

    case GadgetIdentifier::sh_lamp_basic:
      return createSHLampBasic(name, pins, gadget_config);

    case GadgetIdentifier::sh_fan_westinghouse_ir:
      return createSHFanWestinghouseIR(name, pins, gadget_config);

    case GadgetIdentifier::sh_lamp_westinghouse_ir:
      return createSHLampWestinghouseIR(name, pins, gadget_config);

    case GadgetIdentifier::sh_wallswitch_basic:
      return createSHWallswitchBasic(name, pins, gadget_config);

    case GadgetIdentifier::sh_sensor_motion_hr501:
      return createSHSensorMotionHR501(name, pins, gadget_config);

    case GadgetIdentifier::sh_sensor_temperature_dht:
      return createSHSensorTemperatureDHT(name, pins, gadget_config);

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
static std::shared_ptr<SH_Gadget> createGadget(GadgetIdentifier gadget_type, pin_set pins, const std::string& name, JsonObject gadget_config) {
  auto buf_gadget = createGadgetHelper(gadget_type, pins, name, gadget_config);
  if (buf_gadget != nullptr && !buf_gadget->hasInitError()) {
    return buf_gadget;
  }
  logger.println(LOG_TYPE::ERR, "gadget could not be successfully initialized and was discarded");
  return nullptr;
}

/**
 * Function that returns whether the gadget requires IR access
 * @param gadget Gadget type to check
 * @return Whether the stated gadget type needs ir access
 */
static bool gadgetRequiresIR(GadgetIdentifier gadget) {
  switch (gadget) {
    case GadgetIdentifier::sh_fan_westinghouse_ir:
    case GadgetIdentifier::sh_lamp_westinghouse_ir:
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
//    case GadgetIdentifier::sh_lamp_westinghouse_ir:
//      return true;
    default:
      return false;
  }
}