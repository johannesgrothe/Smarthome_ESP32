#include "lamp_rgb.h"

#include <utility>

Lamp_RGB::Lamp_RGB(std::string name) : Gadget(std::move(name),
                                              GadgetType::Lightbulb),
                                       red_(0),
                                       green_(0),
                                       blue_(0) {
}

void Lamp_RGB::setColor(const uint8_t red, const uint8_t green, const uint8_t blue) {
    registerExternalChange();
    red_ = red;
    green_ = green;
    blue_ = blue;
}
