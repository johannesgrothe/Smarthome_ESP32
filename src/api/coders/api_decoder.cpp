#include "api_decoder.h"
#include "config_decoder.h"


GadgetUpdateDTO ApiDecoder::decodeGadgetUpdateInfo(DynamicJsonDocument gadget_data) {
    std::string id = gadget_data["id"];
    const DynamicJsonDocument properties = gadget_data["properties"];
    return {id, properties};
}

std::shared_ptr<SystemConfig> ApiDecoder::decodeSystemConfig(const DynamicJsonDocument &data) {
    return ConfigDecoder::decodeSystemConfig(data);
}

std::shared_ptr<EolConfig> ApiDecoder::decodeEolConfig(const DynamicJsonDocument &data) {
    return ConfigDecoder::decodeEolConfig(data);
}
