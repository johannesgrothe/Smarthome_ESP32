#pragma once

#include <string>
#include <vector>
#include <utility>
#include "../console_logger.h"
#include "request.h"

class SplitRequestBuffer {
private:
  std::vector<std::string> data_buffer_;
  int added_packages_ = 0;

public:
  const int session_id_;
  const std::string path_;
  const std::string sender_;
  const std::string receiver_;
  const int length_;

  SplitRequestBuffer(int session_id, std::string path, std::string sender, std::string receiver, int length);

  void addData(int index, std::string payload);

  std::shared_ptr<Request>getRequest() const;
};