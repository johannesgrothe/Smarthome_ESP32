#pragma once

#include "../datatypes.h"
#include "../storage/system_config.h"
#include "../storage/event_config.h"
#include "../storage/gadget_config.h"
#include "../ip_container.h"
#include "../json_validator.h"

class ConfigDecoder {
public:
  /**
   * Writes a gadget from the config json body
   * @param json_body JSON-data to save the gadget from
   * @return (Whether writing was successful | Status-Message)
   */
  static gadget_tuple decodeGadget(DynamicJsonDocument json_body);

  /**
   * Decodes the json of a system config
   * @param config Config json document to decode
   * @return The decoded config
   */
  static std::shared_ptr<SystemConfig> decodeSystemConfig(DynamicJsonDocument config);

  /**
   * Decodes the json of a event config
   * @param config Config json document to decode
   * @return The decoded config
   */
  static std::shared_ptr<EventConfig> decodeEventConfig(DynamicJsonDocument config);


  /**
   * Decodes the json of a gadget config
   * @param config Config json document to decode
   * @return The decoded config
   */
  static std::shared_ptr<GadgetConfig> decodeGadgetConfig(DynamicJsonDocument config);
};
