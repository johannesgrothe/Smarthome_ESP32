#include "gadget_dto.h"

#include <utility>

GadgetDTO::GadgetDTO(const gadget_definitions::GadgetIdentifier type, std::string name,
                     DynamicJsonDocument properties) : type(type),
                                                       name(std::move(name)),
                                                       properties(std::move(properties)) {
}
