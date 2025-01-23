#include "gadget_update_meta.h"

#include <utility>

GadgetUpdateMeta::GadgetUpdateMeta(std::string id, DynamicJsonDocument properties) : id(std::move(id)),
    properties(std::move(properties)) {
}
