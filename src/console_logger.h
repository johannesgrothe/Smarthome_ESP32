#pragma once

#include <cstring>
#include "user_settings.h"
#include <cstdio>
#include "Arduino.h"
#include "system_settings.h"

#define INDENT_LEN 3

enum class LOG_TYPE {
  INFO, ERR, DATA, NONE, FATAL, WARN
};

class Console_Logger {
protected:

  char core_0_buffer_[LOGGER_MAX_BUFFER_LEN]{};
  char core_0_name_[LOGGER_MAX_NAME]{};
  unsigned int core_0_buffer_ptr_{};
  LOG_TYPE core_0_log_type_;
  bool core_0_has_name_{};
  byte core_0_indent_{};

  char core_1_buffer_[LOGGER_MAX_BUFFER_LEN]{};
  char core_1_name_[LOGGER_MAX_NAME]{};
  unsigned int core_1_buffer_ptr_{};
  LOG_TYPE core_1_log_type_;
  bool core_1_has_name_{};
  byte core_1_indent_{};

  char indent_char_;
  byte indent_len_{};
  bool logging_active_;

  void printIndent() const;

  void printName(const char *) const;

  void printBeginning(LOG_TYPE, bool) const;

public:
  Console_Logger();

  void activateLogging();

  void deactivateLogging();

  void incIndent();

  void decIndent();

  void setName(const char *);

  void setLogType(LOG_TYPE);

  void addToBuffer(const char *);

  void flushBuffer();

  void print(LOG_TYPE, const char *, const char *);

  void print(const char *, const char *);

  void println(const char *, const char *);

  void println(LOG_TYPE, const char *, const char *);

  void print(const char *);

  void print(LOG_TYPE, const char *);

  void println(const char *);

  void println(LOG_TYPE, const char *);

  void add(const char *);

  void add(int);

  void add(unsigned long long);

  void add(float);

  void add(double);

  void add(long);

  void add(unsigned long);

  void add(unsigned int);

  template<class T>
  void addln(T);

  void addln();

};

template<class T>
void Console_Logger::addln(T message) {
  if (!logging_active_)
    return;
  add(message);
  flushBuffer();
}

extern Console_Logger logger;
