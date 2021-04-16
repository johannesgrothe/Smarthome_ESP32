#pragma once
// THIS IS AN AUTO_GENERATED FILE.
// TOUCH IT IF YOU WANT, BUT BEWARE ANY CHANGES COULD BE OVERWRITTEN AT ANY TIME IN THE FUTURE.

#include "datatypes.h"

/**
 * Auto-generated class to contain static configuration info
 */
class Static_Storage {
public:

  /**
   * Returns the static gadget configuration
   * @return a vector containing all gadget information
   */
  static std::vector <gadget_tuple> readAllGadgets() {
    std::vector<gadget_tuple> gadgets;

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

      auto local_gadget = gadget_tuple(gadget_type, remote_bf, pins, gadget_name, gadget_json, code_json);
      gadgets.push_back(local_gadget);
    }
    /*<gadget_block_end>*/

    return gadgets;
  }
};
