#include "lamp_neopixel.h"

#include <utility>

bool Lamp_NeoPixel_RGB::setLedColor(const uint8_t r, const uint8_t g, const uint8_t b) {
    logger_i(getName(), "Setting Color: (%d, %d, %d)", int(r), int(g), int(b));
    const auto color = Adafruit_NeoPixel::Color(r, g, b);
    for (uint16_t k = 0; k < len_; k++) {
        led_stripe_.setPixelColor(k, color);
        // led_stripe_.show();
    }
    led_stripe_.show();
    return true;
}

Lamp_NeoPixel_RGB::Lamp_NeoPixel_RGB(std::string name, const uint8_t pin,
                                     const uint16_t len) : Lamp_RGB(std::move(name)),
                                                           pin_(pin),
                                                           len_(len) {
    logger_e(TAG, "Lamp_NeoPixel_RGB::Lamp_NeoPixel_RGB");
    led_stripe_ = Adafruit_NeoPixel(len_, pin_, NEO_GRB + NEO_KHZ800);
    led_stripe_.begin();
    led_stripe_.clear();
    setLedColor(0x00, 0xFF, 0xFF);
}
