#include "lamp_rgb_connector.h"

LampRGBConnector::LampRGBConnector(Lamp_RGB &subject): GadgetAPIConnector(),
                                                       subject_(subject) {
}

void LampRGBConnector::applyChanges(const DynamicJsonDocument &data) override {
    uint8_t red = subject_.getRed();
    uint8_t green = subject_.getGreen();
    uint8_t blue = subject_.getBlue();

    if (data.containsKey("red")) {
        red = data["red"];
    }

    if (data.containsKey("green")) {
        green = data["green"];
    }

    if (data.containsKey("blue")) {
        blue = data["blue"];
    }

    subject_.setColor(red, green, blue);
}

DynamicJsonDocument LampRGBConnector::encode() override {
    DynamicJsonDocument doc(1000);
    doc["red"] = subject_.getRed();
    doc["green"] = subject_.getRed();
    doc["blue"] = subject_.getRed();
    return doc;
}
