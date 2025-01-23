#pragma once

#include "gadget.h"
#include "../../color.h"

class Lamp_RGB : public Gadget {
protected:
    uint8_t red_, green_, blue_;

public:
    /**
     * Constructor for the Lamp_RGB
     * @param name Name of the gadget
     */
    explicit Lamp_RGB(std::string name);

    void applyChanges(const DynamicJsonDocument &data) override;

    DynamicJsonDocument encodeProperties() override;

    /**
     * Sets the color of the RGB Lamp
     * @param red Red-Value
     * @param green Green-Value
     * @param blue Blue-Value
     */
    void setColor(uint8_t red, uint8_t green, uint8_t blue);

    /**
     * Returns the red part of the current color of the RGB Lamp
     * @return red color part
     */
    uint8_t getRed() const;

    /**
     * Returns the green part of the current color of the RGB Lamp
     * @return green color part
     */
    uint8_t getGreen() const;

    /**
     * Returns the blue part of the current color of the RGB Lamp
     * @return blue color part
     */
    uint8_t getBlue() const;
};
