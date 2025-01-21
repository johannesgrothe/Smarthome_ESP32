#pragma once

#include "../gadget_api_connector.h"
#include "gadgets/abstract/lamp_rgb.h"


class LampRGBConnector : public GadgetAPIConector {
    explicit LampRGBConnector(Lamp_RGB& subject);
};




