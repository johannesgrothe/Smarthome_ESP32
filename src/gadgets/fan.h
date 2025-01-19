#pragma once

#include "gadget.h"

class Fan : public Gadget {
    uint16_t max_level_;
    uint16_t level_;

public:
    /**
     * Constructor for the fan
     * @param name Name of the gadget
     * @param max_level How many levels the fan should operate at
     */
    Fan(const std::string &name, uint16_t max_level);

    /**
     * Sets the fan to operate at a certain level
     * @param level Level to set
     */
    void setLevel(uint16_t level);

    /**
     * @return Level the fan is operating at
     */
    uint16_t getLevel() const;
};
