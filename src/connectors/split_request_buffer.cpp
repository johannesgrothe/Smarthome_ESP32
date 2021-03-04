#include "split_request_buffer.h"

SplitRequestBuffer::SplitRequestBuffer(int session_id, int length):
    data_buffer_(length),
    session_id_(session_id),
    length_(length) {
  logger.println(data_buffer_[length-1].c_str())
};

void SplitRequestBuffer::addData(int index, std::string payload)  {
  if (index >= length_) {
    return;
  }
  //TODO
}

Request *SplitRequestBuffer::getRequest() {
  return nullptr;
}
