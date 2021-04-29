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

  std::cout << "[" << log_lvl << "]" << "[" << tag << "] " << vec << std::endl;
}

#ifdef ARDUINO
#define c_log_w(tag, ...) log_w(__VA_ARGS__)
#else
#define c_log_w(tag, ...) cpp_log("!", tag, __VA_ARGS__)
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

template<class T>
void LoggerState::appendData(T data) {
data_ << data;
}

class Console_Logger {
protected:

std::array<std::shared_ptr<LoggerState>, LOGGER_THREAD_COUNT> logger_states_;

char indent_char_;
bool logging_active_;
std::function<void(LOG_TYPE ,string ,string ,int )> callback_;

static void printOut(string);

static uint32_t getTaskID();

static std::string logLvlToString(LOG_TYPE loglevel);

std::string getIndentationStr(uint8_t indentation) const;

std::string generateMessage(std::shared_ptr<LoggerState> data, uint32_t task_id);

public:
Console_Logger();

void setCallback(std::function<void(LOG_TYPE ,string ,string ,int )>);

void activateLogging();

void deactivateLogging();

Console_Logger& operator++() noexcept;

Console_Logger& operator--() noexcept;

template<class T>
Console_Logger& operator<<(T data) noexcept;

Console_Logger& level(LOG_TYPE log_lvl);

Console_Logger& sender(std::string name);

Console_Logger& log(std::string name, LOG_TYPE log_lvl);

Console_Logger& log(std::string name);

Console_Logger& log(LOG_TYPE log_lvl);

};

template<class T>
Console_Logger& Console_Logger::operator<<(T data) noexcept {
auto thread = getTaskID();
auto state = logger_states_[thread];
std::stringstream sstr;
sstr << data;
auto data_str = sstr.str();
state->appendData(data_str);

if (data_str.back() == '\n') {
  std::string out_msg = generateMessage(state, thread);
  if (logging_active_) {
    printOut(out_msg);
  }
  state->reset();
}
return *this;
}

extern Console_Logger logger;
