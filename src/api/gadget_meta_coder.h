#pragma once

#include "characteristic_meta.h"
#include "gadget_meta.h"
#include "../gadgets/characteristic.h"
#include "../gadgets/gadget.h"

class GadgetMetaEncoder {
public:

  static CharacteristicMeta encodeCharacteristic(Characteristic characteristic);

  static GadgetMeta encodeGadget(const std::shared_ptr<Gadget>& gadget);

};