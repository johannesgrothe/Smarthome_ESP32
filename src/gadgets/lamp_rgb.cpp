#include "lamp_rgb.h"

#include <utility>

Lamp_RGB::Lamp_RGB(std::string name) :
    Gadget(std::move(name),
           GadgetType::Lightbulb,
           {
               Characteristic(CharacteristicIdentifier::status,
                              0,
                              1,
                              1,
                              1),
               Characteristic(CharacteristicIdentifier::brightness,
                              0,
                              100,
                              100,
                              75),
               Characteristic(CharacteristicIdentifier::saturation,
                              0,
                              100,
                              100,
                              75),
               Characteristic(CharacteristicIdentifier::hue,
                              0,
                              360,
                              360,
                              0)
           }) {}
