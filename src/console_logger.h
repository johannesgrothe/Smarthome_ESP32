#ifndef __CONSOLE_LOGGER__
#define __CONSOLE_LOGGER__

#include <Arduino.h>
#include <cstring>

#define PREF_LEN 6
#define INTENT_LEN 3

enum LOG_TYPE {
  LOG_INFO, LOG_ERR, LOG_DATA, LOG_NONE, LOG_FATAL, LOG_WARN
};

class Console_Logger {
protected:
  char intent_char;
  byte intent;
  byte intent_len;

  void print_intent() {
    for (byte k = 0; k < intent; k++) {
      for (byte j = 0; j < intent_len; j++) {
        Serial.print(intent_char);
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
//    for (byte k = strlen(pref); k < PREF_LEN; k++) {
//      Serial.print(' ');
//    }
  }

public:
  Console_Logger() :
    intent_char(' '),
    intent(0),
    intent_len(INTENT_LEN) {
  };

  void incIntent() {
    intent++;
  };

  void decIntent() {
    if (intent > 0)
      intent--;
  };

  void printname(const char *name, const char *message) {
    print_beginning_name(name);
    Serial.print(message);
  }

  void println(const char *name, const char *message) {
    printname(name, message);
    Serial.println();
  }

  void print(const char *message) {
    print(LOG_INFO, message);
  }

  void print(LOG_TYPE type, const char *message) {
    print_beginning(type);
    Serial.print(message);
  }

  void println(const char *message) {
    println(LOG_INFO, message);
  }

  void println(LOG_TYPE type, const char *message) {
    print((LOG_TYPE) type, message);
    Serial.println();
  }

  template<class T>
  void add(T message) {
    Serial.print(message);
  }

  template<class T>
  void addln(T message) {
    Serial.println(message);
  }

  void addln() {
    Serial.println();
  }

//  void printf(LOG_TYPE type, const char * message, int arg) {
//    print_beginning(type);
//    Serial.printf(message, arg);
//  }


//  void printf(LOG_TYPE type, const char *message, va_list args) {
//    va_list arg;
//    print_beginning(type);
//    Serial.printf(message, args);
//  }
//
//  void printf(const char *message, va_list args) {
//    printf(LOG_INFO, message, args);
//  }

};

Console_Logger logger;

#endif //__CONSOLE_LOGGER__