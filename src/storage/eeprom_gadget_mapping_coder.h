#pragma once

#include <sstream>
#include <string>

#include "../console_logger.h"

#include "../datatypes.h"

class EepromGadgetMappingCoder {
private:
  static gadget_event_map decodeMappingPart(const std::string& data);

public:

  static std::string encodeMapping(const std::vector<gadget_event_map>& data);

  static std::vector<gadget_event_map> decodeMapping(const std::string& data);
};