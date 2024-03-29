#pragma once

//Normal Imports
#include <ArduinoJson.h>
#include <memory>
#include <string>
#include "console_logger.h"
#include "storage/system_storage.h"
#include "datatypes.h"

// Gadget
#include "gadgets/gadget.h"

//Lamps
#ifndef UNIT_TEST
#include "gadgets/lamp_neopixel.h"
#include "gadgets/lamp_neopixel_basic.h"
#endif

#include "gadgets/lamp_basic.h"
#include "gadgets/doorbell_basic.h"

//Fan
#include "gadgets/fan_westinghouse_ir.h"
#include "gadgets/lamp_westinghouse_ir.h"

//Switch
#include "gadgets/wallswitch_basic.h"

//Sensors
#ifndef UNIT_TEST
#include "gadgets/sensor_motion_HR501.h"
#include "gadgets/sensor_temperature_DHT.h"
#endif

static const std::vector<gadget_definitions::GadgetIdentifier> IR_GADGETS = {gadget_definitions::GadgetIdentifier::fan_westinghouse_ir,
                                                         gadget_definitions::GadgetIdentifier::lamp_westinghouse_ir};

static const std::vector<gadget_definitions::GadgetIdentifier> RADIO_GADGETS = {};

class GadgetFactory {
private:

  // IR-Gadget to add to created gadgets if necessary
  std::shared_ptr<IR_Gadget> ir_;

  // Radio-Gadget to add to created gadgets if necessary
  std::shared_ptr<Radio_Gadget> radio_;

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
  static std::shared_ptr<Gadget> createGadgetHelper(gadget_definitions::GadgetIdentifier gadget_type,
                                                    port_set pins,
                                                    const std::string &name,
                                                    JsonObject gadget_config);

public:

  GadgetFactory(std::shared_ptr<IR_Gadget> ir,
                std::shared_ptr<Radio_Gadget> radio);

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
  std::shared_ptr<Gadget> createGadget(gadget_definitions::GadgetIdentifier gadget_type,
                                       port_set pins,
                                       const std::string &name,
                                       JsonObject gadget_config,
                                       std::vector<gadget_event_map> event_mapping);

  /**
   * Function that returns whether the gadget requires IR access
   * @param gadget Gadget type to check
   * @return Whether the stated gadget type needs ir access
   */
  static bool gadgetRequiresIR(gadget_definitions::GadgetIdentifier gadget);

  /**
   * Function that returns whether the gadget requires radio access
   * @param gadget Gadget type to check
   * @return Whether the stated gadget type needs radio access
   */
  static bool gadgetRequiresRadio(gadget_definitions::GadgetIdentifier gadget);
};
