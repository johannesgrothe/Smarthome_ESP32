#include <string>
#include <vector>
#include "request.h"

class SplitRequestBuffer {
private:
  std::vector<std::string> data_buffer_;
  int added_packages_ = 0;

public:
  const int session_id_;
  const int length_;

  SplitRequestBuffer(int session_id, int length);

  void addData(int index, std::string payload);

  Request * getRequest() const;
};