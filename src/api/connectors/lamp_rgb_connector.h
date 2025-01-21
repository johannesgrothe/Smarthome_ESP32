#pragma once

#include "../gadget_api_connector.h"
#include "gadgets/abstract/lamp_rgb.h"


class LampRGBConnector : public GadgetAPIConnector {
    Lamp_RGB &subject_;

    explicit LampRGBConnector(Lamp_RGB &subject);
};
