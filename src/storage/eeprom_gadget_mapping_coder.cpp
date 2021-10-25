#include "eeprom_gadget_mapping_coder.h"

std::string EepromGadgetMappingCoder::encodeMapping(const std::vector<gadget_event_map> &data) {
  std::stringstream out_data;
  for (auto mapping_set: data) {
    out_data << '<';
    std::string name = std::get<0>(mapping_set);
    out_data << name;
    out_data << '<';
    bool first_map = true;
    std::vector<gadget_mapping_tuple> mapping_pairs;
    auto mapping = std::get<1>(mapping_set);
    for (auto t: mapping) {
      if (!first_map) {
        out_data << ';';
      } else {
        first_map = false;
      }
      out_data << std::get<0>(t);
      out_data << ';';
      out_data << std::get<1>(t);
    }
    out_data << ">>";
  }
  return out_data.str();
}

std::vector<gadget_event_map> EepromGadgetMappingCoder::decodeMapping(const std::string &data) {
  std::vector<gadget_event_map> out_data;

  std::stringstream out_strm;

  uint8_t indent_level = 0;

  for (char c: data) {
    out_strm << c;
    if (c == '<') {
      indent_level++;
    } else if (c == '>') {
      indent_level--;
      if (indent_level == 0) {
        out_data.push_back(decodeMappingPart(out_strm.str()));
        out_strm = std::stringstream();
      }
    }
  }

  return out_data;
}

gadget_event_map EepromGadgetMappingCoder::decodeMappingPart(const std::string &data) {
  std::vector<gadget_mapping_tuple> out_data;
  std::stringstream buf_strm;
  std::string name;

  bool in_value = false;
  uint16_t buf_charac_id;

  logger_i("EepromGadgetMappingCoder", "Decoding %s", data.c_str());
  for (int i = 1; i < data.size() - 1; i++) {
    char c = data[i];
    if (c == '<') {
      name = buf_strm.str();
      buf_strm = std::stringstream();
    } else if (c == ';' || c == '>') {
      uint16_t buf_val;
      buf_strm >> buf_val;

      if (!in_value) {
        buf_charac_id = buf_val;
      } else {
        gadget_mapping_tuple buf_pair(buf_charac_id, buf_val);
        out_data.push_back(buf_pair);
      }
      in_value = !in_value;
      buf_strm = std::stringstream();
      if (c == '>') {
        break;
      }
    } else {
      buf_strm << c;
    }
  }
  return {name, out_data};
}
