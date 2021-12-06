#pragma once

#include "../console_logger.h"

class DummySerial {
private:
  std::string mock_data_{};
public:

  std::string last_print;

  bool available() {
    return !mock_data_.empty();
  }

  uint8_t read() {
    if (!mock_data_.empty()) {
      char buffer = mock_data_[0];
      std::stringstream buf_strm;
      for (int i = 1; i < mock_data_.size(); i++) {
        buf_strm << mock_data_[i];
      }
      mock_data_ = buf_strm.str();
      return (uint8_t) buffer;
    }
    return 0;
  }

  void printf(const char * format, ...) {
    va_list args;
    va_start (args, format);
    size_t len = vsnprintf(NULL, 0, format, args);
    va_end (args);
    char vec[len + 1];
    va_start (args, format);
    vsnprintf(&vec[0], len + 1, format, args);
    va_end (args);
    std::stringstream sstr;
    sstr << vec;
    last_print = sstr.str();
    logger_i("DummySerial >>", last_print);
  }

  void mockData(const std::string& data) {
    logger_i("DummySerial <<", data);
    mock_data_ = data;
  }
};

inline DummySerial Serial;
