#pragma once

//Normal Imports
#include <ArduinoJson.h>
#include <memory>
#include "console_logger.h"
#include "storage/system_storage.h"

// Gadget
#include "gadgets/sh_gadget.h"

//Lamps
#ifndef UNIT_TEST
#include "gadgets/sh_lamp_neopixel.h"
#include "gadgets/sh_lamp_neopixel_basic.h"
#endif
#include "gadgets/sh_lamp_basic.h"
#include "gadgets/sh_doorbell_basic.h"

//Fan
#include "gadgets/sh_fan_westinghouse_ir.h"
#include "gadgets/sh_lamp_westinghouse_ir.h"

//Wallswitch
#include "gadgets/sh_wallswitch_basic.h"

//Sensors
#ifndef UNIT_TEST
#include "gadgets/sh_sensor_motion_HR501.h"
#include "gadgets/sh_sensor_temperature_DHT.h"
#endif


class GadgetFactory {
private:

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
  static std::shared_ptr <SH_Gadget>
  createGadgetHelper(GadgetIdentifier gadget_type, port_set pins, const std::string &name, JsonObject gadget_config);

public:

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
  static std::shared_ptr <SH_Gadget>
  createGadget(GadgetIdentifier gadget_type, port_set pins, const std::string &name, JsonObject gadget_config);

  /**
   * Function that returns whether the gadget requires IR access
   * @param gadget Gadget type to check
   * @return Whether the stated gadget type needs ir access
   */
  static bool gadgetRequiresIR(GadgetIdentifier gadget);

  /**
   * Function that returns whether the gadget requires radio access
   * @param gadget Gadget type to check
   * @return Whether the stated gadget type needs radio access
   */
  static bool gadgetRequiresRadio(GadgetIdentifier gadget);
};
