#pragma once

#include "gadget_meta.h"
#include "../gadgets/gadget.h"
#include "gadget_update_meta.h"

class GadgetMetaEncoder {
public:

  static GadgetMeta encodeGadget(const std::shared_ptr<Gadget>& gadget);

  static GadgetUpdateMeta encodeGadgetUpdate(const std::shared_ptr<Gadget> &gadget);

};