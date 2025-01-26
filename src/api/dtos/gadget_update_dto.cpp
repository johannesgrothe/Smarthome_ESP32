#include "gadget_update_dto.h"

#include <utility>

GadgetUpdateDTO::GadgetUpdateDTO(std::string id, DynamicJsonDocument properties) : id(std::move(id)),
    properties(std::move(properties)) {
}
