#include "gadget_manager.h"

std::shared_ptr<Radio_Gadget> loadRadioGadget() {
    return nullptr;
}

std::shared_ptr<IR_Gadget> loadIRGadget() {
    // return std::make_shared<IR_Gadget>(ir_recv, ir_send);
    return nullptr;
}

void applyGadgetConfiguration(const std::shared_ptr<GadgetManager> &manager) {
    const auto rgbLamp = std::make_shared<Lamp_NeoPixel_RGB_Basic>("lamp1", 23, 1);
    manager->addGadget(rgbLamp);
}
