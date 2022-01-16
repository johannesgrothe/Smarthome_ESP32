#pragma once

#include "characteristic_meta.h"
#include "gadget_meta.h"
#include "../gadgets/characteristic.h"
#include "../gadgets/gadget.h"
#include "characteristic_update_meta.h"
#include "gadget_update_meta.h"

class GadgetMetaEncoder {
public:

  static CharacteristicMeta encodeCharacteristic(Characteristic characteristic);

  static GadgetMeta encodeGadget(const std::shared_ptr<Gadget>& gadget);

  static CharacteristicUpdateMeta encodeCharacteristicUpdate(Characteristic characteristic);

  static GadgetUpdateMeta encodeGadgetUpdate(const std::shared_ptr<Gadget> &gadget);

};