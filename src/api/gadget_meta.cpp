#include "gadget_meta.h"

#include <utility>

GadgetMeta::GadgetMeta(GadgetIdentifier type, std::string name, std::vector<CharacteristicMeta> characteristics) :
    type(type),
    name(std::move(name)),
    characteristics(std::move(characteristics)) {}
