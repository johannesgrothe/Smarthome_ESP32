#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>
#include "user_settings.h"

#ifndef UNIT_TEST
#include "Arduino.h"
#endif

#define INDENT_LEN 3

using namespace std;

enum class LOG_TYPE {
  INFO, ERR, DATA, NONE, FATAL, WARN
};

class Console_Logger {
protected:

//  char core_0_buffer_[LOGGER_MAX_BUFFER_LEN]{};
//  char core_0_name_[LOGGER_MAX_NAME]{};
//  unsigned int core_0_buffer_ptr_{};
//  LOG_TYPE core_0_log_type_;
//  uint8_t core_0_indent_{};

  string core_0_name_;
  bool core_0_has_name_;
  stringstream core_0_buffer_;
  LOG_TYPE core_0_log_type_;
  uint8_t core_0_indent_{};
  bool callback_status_[8]{false};

//  char core_1_buffer_[LOGGER_MAX_BUFFER_LEN]{};
//  char core_1_name_[LOGGER_MAX_NAME]{};
//  unsigned int core_1_buffer_ptr_{};
//  LOG_TYPE core_1_log_type_;
//  uint8_t core_1_indent_{};

  string core_1_name_;
  bool core_1_has_name_;
  stringstream core_1_buffer_;
  LOG_TYPE core_1_log_type_;
  uint8_t core_1_indent_{};


  char indent_char_;
  uint8_t indent_len_{};
  bool logging_active_;
  std::function<void(LOG_TYPE ,string ,string ,int )> callback_;

  static void printOut(string);

  uint32_t getTaskID();

  void printOut(char);

  void printIndent();

  void printName(string);

  void printBeginning(LOG_TYPE, bool);

  void addToBuffer(string);

  void flushBuffer();

  void setName(const string&);

  void setLogType(LOG_TYPE);

  void callCallback(LOG_TYPE ,string ,string ,int );


public:
  Console_Logger();

  void setCallback(std::function<void(LOG_TYPE ,string ,string ,int )>);

  void setCallbackStatus(LOG_TYPE, bool );

  void activateLogging();

  void deactivateLogging();

  void incIndent();

  void decIndent();

  template<class T>
  void print(T);

  template<class T>
  void print(LOG_TYPE, T);

  template<class T>
  void print(const string&, T);

  template<class T>
  void print(LOG_TYPE, const string&, T);

  template<class T, class ... Args>
  void vprintf(T , va_list);

  template<class T, class ... Args>
  void printf(T, ...);

  template<class T, class ... Args>
  void printf(LOG_TYPE, T, ...);

  void printnl();

  template<class T>
  void println(T);

  template<class T>
  void println(LOG_TYPE, T);

  template<class T>
  void println(const string&, T);

  template<class T>
  void println(LOG_TYPE, const string&, T);

  template<class T, class ... Args>
  void printfln(T, ...);

  template<class T, class ...Args>
  void printfln(LOG_TYPE, T, ...);
};

template<class T>
void Console_Logger::print(T message){
  stringstream ss;
  ss << message;
  addToBuffer(ss.str());
}

template<class T>
void Console_Logger::print(LOG_TYPE type , T message){
  setLogType(type);
  print(message);
}

template<class T>
void Console_Logger::print(const string& name, T message){
  setName(name);
  print(message);
}

template<class T>
void Console_Logger::print(LOG_TYPE type, const string& name, T message){
  setLogType(type);
  setName(name);
  print(message);
}

template<class T, class ... Args>
void Console_Logger::vprintf(T message, va_list ap){
  va_list args;
  stringstream msg;
  char format_string[500];
  va_copy(args, ap);
  vsprintf(&format_string[0], message, ap);
  va_end(ap);
  msg << format_string;
  addToBuffer(msg.str());
}

template<class T, class ... Args>
void Console_Logger::printf(T message, ...){
  va_list ap;
  va_start(ap, message);
  vprintf(message, ap);
  va_end(ap);
}

template<class T, class ... Args>
void Console_Logger::printf(LOG_TYPE type , T message, ...){
  setLogType(type);
  va_list ap;
  va_start(ap, message);
  vprintf(message, ap);
  va_end(ap);
}

template<class T>
void Console_Logger::println(T message) {
  stringstream ss;
  ss << message;
  addToBuffer(ss.str());
  flushBuffer();
}

template<class T>
void Console_Logger::println(LOG_TYPE type, T message){
  setLogType(type);
  println(message);
}

template<class T>
void Console_Logger::println(const string& name, T message){
  setName(name);
  println(message);

}

template<class T>
void Console_Logger::println(LOG_TYPE type, const string& name, T message){
  setLogType(type);
  setName(name);
  println(message);
}

template<class T, class ... Args>
void Console_Logger::printfln(T message, ...){
  va_list ap;
  va_start(ap, message);
  vprintf(message, ap);
  va_end(ap);
  flushBuffer();
}

template<class T, class ... Args>
void Console_Logger::printfln(LOG_TYPE type, T message, ...){
  setLogType(type);
  va_list ap;
  va_start(ap, message);
  vprintf(message, ap);
  va_end(ap);
  flushBuffer();
}

extern Console_Logger logger;
