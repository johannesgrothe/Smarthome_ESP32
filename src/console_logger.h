#pragma once

#include <string>
#include <array>
#include <iostream>
#include <sstream>
#include <functional>
#include <memory>
#include <utility>
#include "user_settings.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstdarg>
#include <cstring>

#ifndef UNIT_TEST
#include "Arduino.h"
#endif

static void cpp_log(std::string log_lvl, std::string tag, std::string format, ...) {
  va_list args;
  va_start (args, format);
  size_t len = vsnprintf(NULL, 0, format.c_str(), args);
  va_end (args);
  char vec[len + 1];
  va_start (args, format);
  vsnprintf(&vec[0], len + 1, format.c_str(), args);
  va_end (args);

  std::cout << "[" << log_lvl << "] " << "[" << tag << "] " << vec << std::endl;
}

#ifdef ARDUINO
#define logger_e(tag, ...) log_e(__VA_ARGS__)
#define logger_w(tag, ...) log_w(__VA_ARGS__)
#define logger_i(tag, ...) log_i(__VA_ARGS__)
#define logger_d(tag, ...) log_d(__VA_ARGS__)
#define logger_v(tag, ...) log_v(__VA_ARGS__)
#else
#define logger_e(tag, ...) cpp_log("err", tag, __VA_ARGS__)
#define logger_w(tag, ...) cpp_log("warn", tag, __VA_ARGS__)
#define logger_i(tag, ...) cpp_log("info", tag, __VA_ARGS__)
#define logger_d(tag, ...) cpp_log("debug", tag, __VA_ARGS__)
#define logger_v(tag, ...) cpp_log("trace", tag, __VA_ARGS__)
#endif
