#include "split_request_buffer.h"

std::string replaceParts(std::string in_str, const std::string& part_to_replace, const std::string& replacement) {
  while (true) {
    auto index = in_str.find(part_to_replace);
    if (index == std::string::npos) {
      return in_str;
    }
    in_str = in_str.replace(index, part_to_replace.length(), replacement);
  }
}

SplitRequestBuffer::SplitRequestBuffer(int session_id, int length):
    data_buffer_(length),
    session_id_(session_id),
    length_(length) {
  logger.println(data_buffer_[length-1].c_str());
}

void SplitRequestBuffer::addData(int index, std::string payload)  {
  if (index >= length_ || index < 0) {
    return;
  }
  if (!data_buffer_[index].empty()) {
    data_buffer_[index] = std::move(payload);
    added_packages_ ++;
  } else {
    logger.printfln("Data at index %d is not empty: '%s'", index, data_buffer_[index].c_str());
  }
}

Request *SplitRequestBuffer::getRequest() const {
  if (added_packages_ == length_) {
    std::string buf_str;
    for (int i = 0; i < length_; i++) {
      std::string part = data_buffer_[i];
      buf_str += part;
    }
    logger.printfln("Received payload: '%s'", buf_str.c_str());
    auto payload_str = replaceParts(buf_str, "$*$", "\"");
    logger.printfln("Received payload: '%s'", payload_str.c_str());
    // TODO
  }
  return nullptr;
}
