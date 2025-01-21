#include "gadget_meta.h"

#include <utility>

GadgetMeta::GadgetMeta(const gadget_definitions::GadgetIdentifier type, std::string name) : type(type),
    name(std::move(name)) {
}
