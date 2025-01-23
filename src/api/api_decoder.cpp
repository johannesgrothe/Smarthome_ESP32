#include "api_decoder.h"
#include "config_decoder.h"


GadgetUpdateMeta ApiDecoder::decodeGadgetUpdateInfo(DynamicJsonDocument gadget_data) {
    // TODO: make safe
    std::string id = gadget_data["id"];
    const auto properties = gadget_data["properties"];
    return {id, properties};
}

std::shared_ptr<SystemConfig> ApiDecoder::decodeSystemConfig(const DynamicJsonDocument &data) {
    return ConfigDecoder::decodeSystemConfig(data);
}

std::shared_ptr<EventConfig> ApiDecoder::decodeEventConfig(const DynamicJsonDocument &data) {
    return ConfigDecoder::decodeEventConfig(data);
}

std::shared_ptr<GadgetConfig> ApiDecoder::decodeGadgetConfig(const DynamicJsonDocument &data) {
    return ConfigDecoder::decodeGadgetConfig(data);
}
