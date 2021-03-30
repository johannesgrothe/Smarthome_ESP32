#pragma once

#include "datatypes.h"

#define STATIC_CONFIG_ACTIVE

class Static_Storage {
public:

/**
 * Reads all gadgets from the eeprom
 * @return a vector containing all gadget information
 */
  static std::vector <gadget_tuple> readAllGadgets() {
    std::vector <gadget_tuple> gadgets;

    /*<gadget_block_start>*/
    {
      pin_set pins = {0, 0, 0, 0, 0};
      bitfield_set remote_bf = {false, false, false, false, false, false, false, false};

      uint8_t gadget_type = /*<gadget_type>*/;

      std::string gadget_name = "/*<gadget_name>*/";
      std::string gadget_json = "/*<gadget_json>*/";
      std::string code_json = "/*<code_json>*/";

      /*<pin_block>*/pins[/*<pin_index>*/] = /*<pin_value>*/;

      /*<remote_block>*/remote_bf[/*<remote_index>*/] = /*<remote_value>*/;

      local_gadget = gadget_tuple(gadget_type, remote_bf, pins, gadget_name, gadget_json, code_json);
      gadgets.append(local_gadget);
    }
    /*<gadget_block_end>*/

    return gadgets;
  }
