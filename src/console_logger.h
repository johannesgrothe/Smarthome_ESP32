#ifndef __CONSOLE_LOGGER__
#define __CONSOLE_LOGGER__

#include <cstring>
#include "user_settings.h"
#include <stdio.h>

#define PREF_LEN 6
#define INTENT_LEN 3

enum LOG_TYPE {
  LOG_INFO, LOG_ERR, LOG_DATA, LOG_NONE, LOG_FATAL, LOG_WARN
};

class Console_Logger {
protected:
  char indent_char;
  byte indent;
  byte indent_len;
  bool logging_active;

  void print_intent() {
    for (byte k = 0; k < indent; k++) {
      for (byte j = 0; j < indent_len; j++) {
        Serial.print(indent_char);
      }
    }
  }

  void print_beginning_name(const char *name) {
    print_intent();
    Serial.printf("[%s] ", name);
  }

  void print_beginning(LOG_TYPE type) {
    char pref[PREF_LEN]{};
    switch (type) {
      case LOG_INFO:
        strcpy(pref, "[i]");
        break;
      case LOG_ERR:
        strcpy(pref, "[x]");
        break;
      case LOG_DATA:
        strcpy(pref, "->");
        break;
      case LOG_NONE:
        strcpy(pref, "");
        break;
      case LOG_FATAL:
        strcpy(pref, "[:/]");
      case LOG_WARN:
        strcpy(pref, "[!]");
        break;
      default:
        break;
    }

    print_intent();

    Serial.print(pref);
    Serial.print(' ');
  }

public:
  Console_Logger() :
    indent_char(' '),
    indent(0),
    indent_len(INTENT_LEN),
    logging_active(LOGGING_ACTIVE) {
  };

  void activateLogging() {
    logging_active = true;
    println("Activating Logger");
  }

  void deactivateLogging() {
    println("Deactivating Logger");
    logging_active = false;
  }

  void incIntent() {
    indent++;
  };

  void decIntent() {
    if (indent > 0)
      indent--;
  };

  void printname(const char *name, const char *message) {
    if (!logging_active)
      return;
    print_beginning_name(name);
    Serial.print(message);
  }

  void println(const char *name, const char *message) {
    if (!logging_active)
      return;
    printname(name, message);
    Serial.println();
  }

  void print(const char *message) {
    if (!logging_active)
      return;
    print(LOG_INFO, message);
  }

  void print(LOG_TYPE type, const char *message) {
    if (!logging_active)
      return;
    print_beginning(type);
    Serial.print(message);
  }

  void println(const char *message) {
    if (!logging_active)
      return;
    println(LOG_INFO, message);
  }

  void println(LOG_TYPE type, const char *message) {
    if (!logging_active)
      return;
    print((LOG_TYPE) type, message);
    Serial.println();
  }

  template<class T>
  void add(T message) {
    if (!logging_active)
      return;
    Serial.print(message);
  }

  template<class T>
  void addln(T message) {
    if (!logging_active)
      return;
    Serial.println(message);
  }

  void addln() {
    if (!logging_active)
      return;
    Serial.println();
  }

  void add(unsigned long data, int basis) {
    if (!logging_active)
      return;
    Serial.print(data, basis);
  }

  void addln(unsigned long data, int basis) {
    if (!logging_active)
      return;
    add(data, basis);
    Serial.println();
  }

  void printf(LOG_TYPE type, const char *message, ...) {
    va_list arglist;
    va_start( arglist, message );
    printf(message, arglist);
    va_end( arglist );
  }

  void printf(LOG_TYPE type, const char *message, va_list arglist) {
    const short str_len = strlen(message);
    char buffer[str_len]{};
    print_beginning(type);
    vsnprintf (buffer, str_len, message, arglist);
    Serial.printf(buffer);
  }


  void printf(const char *message, ...) {
    va_list arglist;
    va_start( arglist, message );
    printf(LOG_INFO, message, arglist);
    va_end( arglist );
  }

} /*extern logger*/;

Console_Logger logger;

#endif //__CONSOLE_LOGGER__