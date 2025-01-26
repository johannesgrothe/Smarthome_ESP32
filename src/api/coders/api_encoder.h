#pragma once

#include <vector>
#include <ArduinoJson.h>
#include <api/dtos/gadget_dto.h>
#include <api/dtos/client_dto.h>
#include <api/dtos/gadget_update_dto.h>

class ApiEncoder {
public:

  /**
   * Encodes system information data of a client into a json document compliant to the api specification
   *
   * @param client_data Data of the client to encode
   * @return A json document containing the client data
   */
  static DynamicJsonDocument encodeClient(const ClientDTO &client_data, uint16_t runtime_id);

  /**
   * Encodes a set of gadget data into a json document compliant to the api specification
   *
   * @param gadget_data Data of the gadget to encode
   * @return A json document containing the gadget data
   */
  static DynamicJsonDocument encodeGadget(const GadgetDTO &gadget_data);

  /**
   * Encodes the data needed to perform a sync to api specification
   *
   * @param client_data Data of the client
   * @param runtime_id
   * @param gadgets Datasets of all the gadgets
   * @return A json document containing all the information
   */
  static DynamicJsonDocument encodeSync(const ClientDTO &client_data,
                                        const std::vector<GadgetDTO> &gadgets,
                                        uint16_t runtime_id);

  /**
   * Encodes the data needed to update information for a specific gadget on the bridge
   *
   * @param data Data of the gadget
   * @return A json document containing all the information
   */
  static DynamicJsonDocument encodeGadgetUpdate(const GadgetUpdateDTO &data);

  /**
   * Encodes the data for the periodic heartbeat message
   *
   * @param runtime_id Runtime ID of the system
   * @return A json document containing all the information
   */
  static DynamicJsonDocument encodeHeartbeat(uint16_t runtime_id);
};
