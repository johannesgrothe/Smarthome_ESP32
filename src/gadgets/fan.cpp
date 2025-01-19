#include "fan.h"


Fan::Fan(const std::string &name, const uint16_t max_level) : Gadget(name,
                                                               GadgetType::Fan),
                                                        max_level_(max_level),
                                                        level_(0) {
}

void Fan::setLevel(const uint16_t level) {
    registerExternalChange();
    level_ = level;
}


uint16_t Fan::getLevel() const {
    return level_;
}
