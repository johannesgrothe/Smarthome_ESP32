#pragma once

#include "gadget.h"

/**
 * Class that implements a simple doorbell
 */
class Doorbell : public Gadget {
public:
    explicit Doorbell(const std::string &name);

    /**
     * Registers a triggered doorbell and throws event
     */
    void doorbellTriggered();
};
