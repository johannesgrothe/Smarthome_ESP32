#pragma once
#include <gadgets/impl/lamp_neopixel_basic.h>
#include "temp_variants.h"
#include "connectors/radio_gadget.h"
#include "connectors/ir_gadget.h"
#include "gadget_manager.h"

constexpr SwVariant SW_VARIANT = SwVariant::single_neopixel;

std::shared_ptr<Lamp_NeoPixel_RGB_Basic> lamp1;

inline std::shared_ptr<Radio_Gadget> loadRadioGadget() {
    return nullptr;
}

inline std::shared_ptr<IR_Gadget> loadIRGadget() {
    return nullptr;
}

inline void loadGadgets(const std::shared_ptr<GadgetManager> &manager) {
    lamp1 = std::make_shared<Lamp_NeoPixel_RGB_Basic>("lamp1", 23, 1);
    manager->addGadget(lamp1);
}
