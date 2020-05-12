#ifndef __CONSOLE_LOGGER__
#define __CONSOLE_LOGGER__

#include <cstring>
#include "user_settings.h"
#include <cstdio>
#include "Arduino.h"
#include "system_settings.h"

#define PREF_LEN 6
#define INTENT_LEN 3

enum LOG_TYPE {
  LOG_INFO, LOG_ERR, LOG_DATA, LOG_NONE, LOG_FATAL, LOG_WARN
};

class Console_Logger {
protected:

  char core_0_buffer[LOGGER_MAX_BUFFER_LEN]{};
  char core_0_name[LOGGER_MAX_NAME]{};
  int core_0_buffer_ptr;
  LOG_TYPE core_0_log_type;
  bool core_0_has_name;


  char core_1_buffer[LOGGER_MAX_BUFFER_LEN]{};
  char core_1_name[LOGGER_MAX_NAME]{};
  int core_1_buffer_ptr;
  LOG_TYPE core_1_log_type;
  bool core_1_has_name;

  char indent_char;
  byte indent{};
  byte indent_len{};
  bool logging_active;

  void printIndent() const {
    if (xPortGetCoreID() == 0 || xPortGetCoreID() == 1) {
      Serial.printf("%d | ", xPortGetCoreID());
    } else {
      Serial.print("? | ");
    }
    for (byte k = 0; k < indent; k++) {
      for (byte j = 0; j < indent_len; j++) {
        Serial.print(indent_char);
      }
    }
  }

  static void printName(const char *name) {
    Serial.printf("[%s] ", name);
  }

  static void printBeginning(LOG_TYPE type, bool complete = true) {
    switch (type) {
      case LOG_INFO:
        if (complete)
          Serial.print("[i]");
        break;
      case LOG_ERR:
        Serial.print("[x]");
        break;
      case LOG_DATA:
        Serial.print("->");
        break;
      case LOG_NONE:
        break;
      case LOG_FATAL:
        Serial.print("[:/]");
      case LOG_WARN:
        Serial.print("[!]");
        break;
      default:
        break;
    }
    Serial.print(" ");
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

  void incIndent() {
    indent++;
  };

  void decIndent() {
    if (indent > 0)
      indent--;
  };

  void setName(const char *name) {
    if (xPortGetCoreID() == 0) {
      strncpy(core_0_name, name, LOGGER_MAX_NAME);
      core_0_has_name = true;
    } else {
      strncpy(core_1_name, name, LOGGER_MAX_NAME);
      core_0_has_name = true;
    }
  }

  void setLogType(LOG_TYPE type) {
    if (xPortGetCoreID() == 0) {
      core_0_log_type = type;
    } else {
      core_1_log_type = type;
    }
  }

  void addToBuffer(const char *message) {
    if (xPortGetCoreID() == 0) {
      int remaining_space = ((LOGGER_MAX_BUFFER_LEN - core_0_buffer_ptr) - 2);
      int print_len = strlen(message) < remaining_space ? strlen(message) : remaining_space;
      strcpy(&core_0_buffer[0] + core_0_buffer_ptr, message);
      core_0_buffer_ptr += print_len;
    } else {
      int remaining_space = ((LOGGER_MAX_BUFFER_LEN - core_1_buffer_ptr) - 2);
      unsigned int print_len = strlen(message) < remaining_space ? strlen(message) : remaining_space;
      strcpy(&core_1_buffer[0] + core_1_buffer_ptr, message);
      core_1_buffer_ptr += (int) print_len;
    }
  }

  void flushBuffer() {
    if (xPortGetCoreID() == 0) {
      if (core_0_buffer_ptr != 0) {
        printIndent();
        if (core_0_has_name) {
          printBeginning(core_0_log_type, false);
          printName(&core_0_name[0]);
        } else {
          printBeginning(core_0_log_type, true);
        }
        Serial.println(core_0_buffer);
      }
      core_0_has_name = false;
      core_0_buffer_ptr = 0;
      core_0_log_type = LOG_NONE;
    } else {
      if (core_1_buffer_ptr != 0) {
        printIndent();
        if (core_1_has_name) {
          printBeginning(core_1_log_type, false);
          printName(&core_1_name[0]);
        } else {
          printBeginning(core_1_log_type, true);
        }
        Serial.println(core_1_buffer);
      }
      core_1_has_name = false;
      core_1_buffer_ptr = 0;
      core_1_log_type = LOG_NONE;
    }
  }

  void print(LOG_TYPE type, const char *name, const char *message) {
    if (!logging_active)
      return;
    if (xPortGetCoreID() == 0) {
      if (core_0_buffer_ptr != 0) {
        flushBuffer();
        println(LOG_ERR, "Logger Error: Buffer not flushed.");
      }
    } else {
      if (core_1_buffer_ptr != 0) {
        flushBuffer();
        println(LOG_ERR, "Logger Error: Buffer not flushed.");
      }
    }
    setName(name);
    setLogType(LOG_INFO);
    addToBuffer(message);
  }

  void print(const char *name, const char *message) {
    print(LOG_INFO, name, message);
  }

  void println(const char *name, const char *message) {
    println(LOG_INFO, name, message);
  }

  void println(LOG_TYPE type, const char *name, const char *message) {
    if (!logging_active)
      return;
    print(name, message);
    flushBuffer();
  }

  void print(const char *message) {
    if (!logging_active)
      return;
    print(LOG_INFO, message);
  }

  void print(LOG_TYPE type, const char *message) {
    if (!logging_active)
      return;
    setLogType(type);
    addToBuffer(message);
  }

  void println(const char *message) {
    if (!logging_active)
      return;
    println(LOG_INFO, message);
  }

  void println(LOG_TYPE type, const char *message) {
    if (!logging_active)
      return;
    print(type, message);
    flushBuffer();
  }

  void add(const char *message) {
    addToBuffer(message);
  }

  void add(int message) {
    char buffer[LOGGER_T_BUFFER_LEN]{};
    sprintf(buffer, "%d", message);
    addToBuffer(buffer);
  }

  void add(const unsigned long long message) {
    unsigned long long len_count_buffer = message;

    unsigned int num_len;
    if (message == 0) {
      num_len = 2;
    } else {
      num_len = 1;
      while (len_count_buffer > 0) {
        num_len++;
        len_count_buffer = len_count_buffer / 10;
      }
    }

    unsigned long long buf = message;
    char buffer[num_len];

    unsigned int pointer = num_len - 2;
    while (buf > 0) {
      buffer[pointer] = int(buf % 10) + 48;
      buf = buf / 10;
      pointer--;
    }
    buffer[num_len - 1] = '\0';
    addToBuffer(buffer);
  }

  void add(float message) {
    char buffer[LOGGER_T_BUFFER_LEN]{};
    sprintf(buffer, "%f", message);
    addToBuffer(buffer);
  }

  void add(double message) {
    char buffer[LOGGER_T_BUFFER_LEN]{};
    sprintf(buffer, "%lf", message);
    addToBuffer(buffer);
  }

  void add(long message) {
    char buffer[LOGGER_T_BUFFER_LEN]{};
    sprintf(buffer, "%ld", message);
    addToBuffer(buffer);
  }

  void add(unsigned long message) {
    char buffer[LOGGER_T_BUFFER_LEN]{};
    sprintf(buffer, "%lu", message);
    addToBuffer(buffer);
  }

  void add(unsigned int message) {
    char buffer[LOGGER_T_BUFFER_LEN]{};
    sprintf(buffer, "%u", message);
    addToBuffer(buffer);
  }

  template<class T>
  void addln(T message) {
    if (!logging_active)
      return;
    add(message);
    flushBuffer();
  }

  void addln() {
    if (!logging_active)
      return;
    flushBuffer();
  }

} /*extern logger*/;

Console_Logger logger;

#endif //__CONSOLE_LOGGER__