#include "gadget_meta_coder.h"


GadgetMeta GadgetMetaEncoder::encodeGadget(const std::shared_ptr<Gadget> &gadget) {
    return {
        gadget_definitions::GadgetIdentifier::fan_westinghouse_ir,
        gadget->getName()
    };
}


GadgetUpdateMeta GadgetMetaEncoder::encodeGadgetUpdate(const std::shared_ptr<Gadget> &gadget) {
    return {
        gadget->getName()
    };
}
