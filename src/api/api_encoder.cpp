#include "api_encoder.h"

#include <sstream>

DynamicJsonDocument ApiEncoder::encodeClient(const ClientMeta &client_data, uint16_t runtime_id) {
    DynamicJsonDocument doc(350);
    doc["runtime_id"] = runtime_id;
    doc["boot_mode"] = static_cast<int>(client_data.boot_mode);
    doc["sw_uploaded"] = client_data.sw_uploaded;
    doc["sw_commit"] = client_data.sw_commit;
    doc["sw_branch"] = client_data.sw_branch;

    std::stringstream api_version_strm;
    api_version_strm << static_cast<int>(client_data.api_version_major) << "." << static_cast<int>(client_data.
        api_version_minor) << "." << static_cast<int>(client_data.api_version_bugfix);
    doc["api_version"] = api_version_strm.str();
    return doc;
}

DynamicJsonDocument ApiEncoder::encodeGadget(const GadgetMeta &gadget_data) {
    DynamicJsonDocument doc(350);
    doc["type"] = static_cast<int>(gadget_data.type);
    doc["id"] = gadget_data.name;
    doc["properties"] = gadget_data.properties;
    return doc;
}

DynamicJsonDocument ApiEncoder::encodeSync(const ClientMeta &client_data,
                                           const std::vector<GadgetMeta> &gadget_data,
                                           const uint16_t runtime_id) {
    DynamicJsonDocument doc(800);
    doc["client"] = encodeClient(client_data, runtime_id);
    doc.createNestedArray("gadgets");
    const JsonArray gadgets = doc["gadgets"];
    for (const auto &gadget: gadget_data) {
        gadgets.add(encodeGadget(gadget));
    }
    return doc;
}

DynamicJsonDocument ApiEncoder::encodeGadgetUpdate(const GadgetUpdateMeta &data) {
    DynamicJsonDocument doc(300);
    doc["id"] = data.id;
    doc["properties"] = data.properties;
    return doc;
}

DynamicJsonDocument ApiEncoder::encodeHeartbeat(uint16_t runtime_id) {
    DynamicJsonDocument doc(100);
    doc["runtime_id"] = runtime_id;
    return doc;
}
