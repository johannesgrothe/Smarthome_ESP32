#pragma once
#include <api/connectors/lamp_rgb_connector.h>
#include <gadgets/impl/lamp_neopixel_basic.h>

auto lamp1 = Lamp_NeoPixel_RGB_Basic("lamp1", 23, 1);

inline std::shared_ptr<Radio_Gadget> loadRadioGadget() {
    return nullptr;
}

inline std::shared_ptr<IR_Gadget> loadIRGadget() {
    // return std::make_shared<IR_Gadget>(ir_recv, ir_send);
    return nullptr;
}

inline void applyGadgetConfiguration(const std::shared_ptr<GadgetManager> &manager) {
    auto lamp1_connector = LampRGBConnector(lamp1);
    manager->addGadget(lamp1);
}
