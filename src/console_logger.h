#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <functional>
#include "user_settings.h"
#include <cstdio>
#include "Arduino.h"
#include "system_settings.h"

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
//  byte core_0_indent_{};

  string core_0_name_;
  bool core_0_has_name_;
  stringstream core_0_buffer_;
  LOG_TYPE core_0_log_type_;
  byte core_0_indent_{};

//  char core_1_buffer_[LOGGER_MAX_BUFFER_LEN]{};
//  char core_1_name_[LOGGER_MAX_NAME]{};
//  unsigned int core_1_buffer_ptr_{};
//  LOG_TYPE core_1_log_type_;
//  byte core_1_indent_{};

  string core_1_name_;
  bool core_1_has_name_;
  stringstream core_1_buffer_;
  LOG_TYPE core_1_log_type_;
  byte core_1_indent_{};


  char indent_char_;
  byte indent_len_{};
  bool logging_active_;
  std::function<void(LOG_TYPE ,string ,string ,int )> callback_;


  static void printOut(string);

  void printOut(char );

  void printIndent();

  void printName(string);

  void printBeginning(LOG_TYPE, bool);

  void addToBuffer(string);

  void flushBuffer();

  void setName(const string&);

  void setLogType(LOG_TYPE);


public:
  Console_Logger();

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

  void println();

  template<class T>
  void println(T);

  template<class T>
  void println(LOG_TYPE, T);

  template<class T>
  void println(const string&, T);

  template<class T>
  void println(LOG_TYPE, const string&, T);


};

template<class T>
void Console_Logger::print(T message){
  stringstream ss;
  ss << message << "\n";
  addToBuffer(ss.str());
}

template<class T>
void Console_Logger::print(LOG_TYPE type , T message){
  setLogType(type);
  stringstream ss;
  ss << message << "\n";
  addToBuffer(ss.str());
}

template<class T>
void Console_Logger::print(const string& name, T message){
  setName(name);
  stringstream ss;
  ss << message << "\n";
  addToBuffer(ss.str());
}

template<class T>
void Console_Logger::print(LOG_TYPE type, const string& name, T message){
  setLogType(type);
  setName(name);
  stringstream ss;
  ss << message << "\n";
  addToBuffer(ss.str());
}

template<class T>
void Console_Logger::println(T message) {
  stringstream ss;
  ss << message << "\n";
  addToBuffer(ss.str());
  flushBuffer();
}

template<class T>
void Console_Logger::println(LOG_TYPE type, T message){
  setLogType(type);
  stringstream ss;
  ss << message << "\n";
  addToBuffer(ss.str());
  flushBuffer();
}

template<class T>
void Console_Logger::println(const string& name, T message){
  setName(name);
  stringstream ss;
  ss << message << "\n";
  addToBuffer(ss.str());
  flushBuffer();
}

template<class T>
void Console_Logger::println(LOG_TYPE type, const string& name, T message){
  setLogType(type);
  setName(name);
  stringstream ss;
  ss << message << "\n";
  addToBuffer(ss.str());
  flushBuffer();
}

extern Console_Logger logger;
