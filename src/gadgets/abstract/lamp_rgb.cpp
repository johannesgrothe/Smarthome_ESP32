#include "lamp_rgb.h"

#include <utility>

Lamp_RGB::Lamp_RGB(std::string name) : Gadget(std::move(name),
                                              GadgetType::Lightbulb),
                                       red_(0),
                                       green_(0),
                                       blue_(0) {
}

void Lamp_RGB::applyChanges(const DynamicJsonDocument &data) {
    uint8_t red = getRed();
    uint8_t green = getGreen();
    uint8_t blue = getBlue();

    if (data.containsKey("red")) {
        red = data["red"];
    }

    if (data.containsKey("green")) {
        green = data["green"];
    }

    if (data.containsKey("blue")) {
        blue = data["blue"];
    }

    setColor(red, green, blue);
}

DynamicJsonDocument Lamp_RGB::encodeProperties() {
    DynamicJsonDocument doc(75);
    doc["red"] = getRed();
    doc["green"] = getRed();
    doc["blue"] = getRed();
    return doc;
}

void Lamp_RGB::setColor(const uint8_t red, const uint8_t green, const uint8_t blue) {
    registerExternalChange();
    red_ = red;
    green_ = green;
    blue_ = blue;
}

uint8_t Lamp_RGB::getRed() const {
    return red_;
}


uint8_t Lamp_RGB::getGreen() const {
    return green_;
}


uint8_t Lamp_RGB::getBlue() const {
    return blue_;
}
