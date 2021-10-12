#include "gadget_config.h"

GadgetConfig::GadgetConfig(std::vector<gadget_tuple> gadget_data) :
    gadgets(std::move(gadget_data)) {}
