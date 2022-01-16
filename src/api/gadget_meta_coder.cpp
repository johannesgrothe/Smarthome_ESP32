#include "gadget_meta_coder.h"

CharacteristicMeta GadgetMetaEncoder::encodeCharacteristic(Characteristic characteristic) {
  return {characteristic.type,
          characteristic.min,
          characteristic.max,
          characteristic.getStepValue(),
          characteristic.steps};
}

GadgetMeta GadgetMetaEncoder::encodeGadget(const std::shared_ptr<Gadget> &gadget) {
  std::vector<CharacteristicMeta> out_characteristics;
  for (auto c: gadget->getCharacteristics()) {
    out_characteristics.push_back(GadgetMetaEncoder::encodeCharacteristic(c));
  }
  return {gadget_definitions::GadgetIdentifier::fan_westinghouse_ir,
          gadget->getName(),
          out_characteristics};
}

CharacteristicUpdateMeta GadgetMetaEncoder::encodeCharacteristicUpdate(Characteristic characteristic) {
  return {characteristic.type,
          characteristic.getStepValue()};
}

GadgetUpdateMeta GadgetMetaEncoder::encodeGadgetUpdate(const std::shared_ptr<Gadget> &gadget) {
  std::vector<CharacteristicUpdateMeta> out_cs;
  for (auto c: gadget->getCharacteristics()) {
    out_cs.push_back(encodeCharacteristicUpdate(c));
  }
  return {gadget->getName(),
          out_cs};
}
