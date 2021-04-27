#include "split_request_buffer.h"

#include <utility>

std::string replaceParts(std::string in_str, const std::string &part_to_replace, const std::string &replacement) {
  if (part_to_replace.find(replacement) != std::string::npos) {
    logger.sender("SplitRequestBuffer").level(LOG_TYPE::ERR) << "Illegal replacement string\n";
    return "";
  }
  while (true) {
    auto index = in_str.find(part_to_replace);
    if (index == std::string::npos) {
      return in_str;
    }
    in_str = in_str.replace(index, part_to_replace.length(), replacement);
  }
}

SplitRequestBuffer::SplitRequestBuffer(int session_id, std::string path, std::string sender, std::string receiver,
                                       int length) :
    data_buffer_(length),
    session_id_(session_id),
    path_(std::move(path)),
    sender_(std::move(sender)),
    receiver_(std::move(receiver)),
    length_(length) {}

void SplitRequestBuffer::addData(int index, std::string payload) {
  if (index >= length_ || index < 0) {
    return;
  }
  if (data_buffer_[index].empty()) {
    data_buffer_[index] = std::move(payload);
    added_packages_++;
  } else {
    logger.sender("SplitRequestBuffer").level(LOG_TYPE::ERR) << "Data at index " << index
                                                                   << " is not empty: " << data_buffer_[index].c_str()
                                                                   << "\n";
  }
}

std::shared_ptr<Request> SplitRequestBuffer::getRequest() const {
  if (added_packages_ == length_) {
    std::string buf_str;
    for (int i = 0; i < length_; i++) {
      std::string part = data_buffer_[i];
      buf_str += part;
    }
    // Replace coded data from string to decodable json data
    auto payload_str = replaceParts(buf_str, "$*$", "\"");
    DynamicJsonDocument doc(5000);
    auto serialization_ok = ArduinoJson::deserializeJson(doc, payload_str);

    // Check if deserialization was successful
    if (serialization_ok != DeserializationError::Ok) {
      logger.sender("SplitRequestBuffer").level(LOG_TYPE::ERR)
          << "Error in split request deserialization process\n";
      return nullptr;
    }

    auto out_req = std::make_shared<Request>(path_,
                                             session_id_,
                                             sender_,
                                             receiver_,
                                             doc);

    return out_req;

  }
  return nullptr;
}
