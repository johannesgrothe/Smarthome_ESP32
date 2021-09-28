#pragma once

#include <vector>
#include <ArduinoJson.h>
#include "gadget_meta.h"
#include "client_meta.h"

class ApiEncoder {
public:

  /**
   * Encodes system information data of a client into a json document compliant to the api specification
   *
   * @param client_data Data of the client to encode
   * @return A json document containing the client data
   */
  static DynamicJsonDocument encodeClient(const ClientMeta& client_data);

  /**
   * Encodes a set of gadget data into a json document compliant to the api specification
   *
   * @param gadget_data Data of the gadget to encode
   * @return A json document containing the gadget data
   */
  static DynamicJsonDocument encodeGadget(const GadgetMeta& gadget_data);

  /**
   *
   * @param gadget_data
   * @return
   */
  static DynamicJsonDocument encodeCharacteristic(CharacteristicMeta characteristic_data);

  /**
   * Encodes the data needed to perform a sync to api specification
   *
   * @param client_data Data of the client
   * @param gadget_data Datasets of all the gadgets
   * @return A json document containing all the information
   */
  static DynamicJsonDocument encodeSync(const ClientMeta& client_data, const std::vector<GadgetMeta> &gadgets);
};
