#pragma once

#include <string>
#include <array>
#include <iostream>
#include <sstream>
#include <functional>
#include <memory>
#include <utility>
#include "user_settings.h"

#ifndef UNIT_TEST
#include "Arduino.h"
#endif

#define INDENT_LEN 3
#define LOGGER_THREAD_COUNT 5

using namespace std;

enum class LOG_TYPE {
  INFO, ERR, DATA, NONE, FATAL, WARN
};

class LoggerState {
private:
  LOG_TYPE loglevel_;
  std::string sender_;
  uint8_t indentation_;
  std::stringstream data_;

public:

  LoggerState();

  void reset();

  template<class T>
  void appendData(T data);

  std::string getData() const;

  void resetData();

  std::string getSender() const;

  void setSender(std::string sender);

  void resetSender();

  LOG_TYPE getLevel();

  void setLevel(LOG_TYPE loglevel);

  void resetLevel();

  void incIndent();

  void decIndent();

  void setIndent(uint8_t indentation);

  uint8_t getIndent() const;
};

class Console_Logger {
protected:

  std::array<std::shared_ptr<LoggerState>, LOGGER_THREAD_COUNT> logger_states_;

  char indent_char_;
  bool logging_active_;
  std::function<void(LOG_TYPE ,string ,string ,int )> callback_;

  static void printOut(string);

  uint32_t getTaskID();

  std::string logLvlToString(LOG_TYPE loglevel);

  std::string getIndentationStr(uint8_t indentation) const;

  void flushData(std::shared_ptr<LoggerState> data, uint32_t task_id);

public:
  Console_Logger();

  void setCallback(std::function<void(LOG_TYPE ,string ,string ,int )>);

  void activateLogging();

  void deactivateLogging();

  Console_Logger& operator++() noexcept;

  Console_Logger& operator--() noexcept;

  Console_Logger& operator<<(std::string data) noexcept;

//  template<class T>
//  Console_Logger& operator<<(T data) noexcept;

  Console_Logger& setLevel(LOG_TYPE log_lvl);

  Console_Logger& setSender(std::string name);

};

extern Console_Logger logger;
