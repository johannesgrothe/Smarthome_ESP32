#include <ArduinoJson.h>
#include "gadgets/abstract/gadget.h"

class GadgetAPIConector {
    const Gadget& subject_;

public:
    explicit GadgetAPIConector(const Gadget& subject);

    void applyChanges(DynamicJsonDocument data) const;

    DynamicJsonDocument encode() const;
};
