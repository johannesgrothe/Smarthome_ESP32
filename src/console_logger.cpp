#include "console_logger.h"

void Console_Logger::printIndent() const {
  if (xPortGetCoreID() == 0 || xPortGetCoreID() == 1) {
    Serial.printf("%d | ", xPortGetCoreID());
  } else {
    Serial.print("? | ");
  }

  byte local_indent;
  if (xPortGetCoreID() == 0) {
    local_indent = core_0_indent_;
  } else {
    local_indent = core_1_indent_;
  }

  for (byte k = 0; k < local_indent; k++) {
    for (byte j = 0; j < indent_len_; j++) {
      Serial.print(indent_char_);
    }
  }
}

void Console_Logger::printName(const char *name) const {
  Serial.printf("[%s] ", name);
}

void Console_Logger::printBeginning(const LOG_TYPE type, const bool complete = true) const {
  switch (type) {
    case LOG_TYPE::INFO:
      if (complete)
        Serial.print("[i]");
      break;
    case LOG_TYPE::ERR:
      Serial.print("[x]");
      break;
    case LOG_TYPE::DATA:
      Serial.print("->");
      break;
    case LOG_TYPE::NONE:
      break;
    case LOG_TYPE::FATAL:
      Serial.print("[:/]");
    case LOG_TYPE::WARN:
      Serial.print("[!]");
      break;
    default:
      break;
  }
  Serial.print(" ");
}

Console_Logger::Console_Logger() :
  core_0_log_type_(LOG_TYPE::INFO),
  core_0_indent_(0),
  core_1_log_type_(LOG_TYPE::INFO),
  core_1_indent_(0),
  indent_char_(' '),
  indent_len_(INDENT_LEN),
  logging_active_(LOGGING_ACTIVE) {
};

void Console_Logger::activateLogging() {
  logging_active_ = true;
  println("Activating Logger");
}

void Console_Logger::deactivateLogging() {
  println("Deactivating Logger");
  logging_active_ = false;
}

void Console_Logger::incIndent() {
  if (xPortGetCoreID() == 0) {
    core_0_indent_++;
  } else {
    core_1_indent_++;
  }
};

void Console_Logger::decIndent() {
  if (xPortGetCoreID() == 0) {
    if (core_0_indent_ > 0)
      core_0_indent_--;
  } else {
    if (core_1_indent_ > 0)
      core_1_indent_--;
  }
};

void Console_Logger::setName(const char *name) {
  if (xPortGetCoreID() == 0) {
    strncpy(core_0_name_, name, LOGGER_MAX_NAME);
    core_0_has_name_ = true;
  } else {
    strncpy(core_1_name_, name, LOGGER_MAX_NAME);
    core_0_has_name_ = true;
  }
}

void Console_Logger::setLogType(const LOG_TYPE type) {
  if (xPortGetCoreID() == 0) {
    core_0_log_type_ = type;
  } else {
    core_1_log_type_ = type;
  }
}

void Console_Logger::addToBuffer(const char *message) {
  if (xPortGetCoreID() == 0) {
    unsigned int remaining_space = ((LOGGER_MAX_BUFFER_LEN - core_0_buffer_ptr_) - 2);
    unsigned int print_len = strlen(message) < remaining_space ? strlen(message) : remaining_space;
    strcpy(&core_0_buffer_[0] + core_0_buffer_ptr_, message);
    core_0_buffer_ptr_ += print_len;
  } else {
    unsigned int remaining_space = ((LOGGER_MAX_BUFFER_LEN - core_1_buffer_ptr_) - 2);
    unsigned int print_len = strlen(message) < remaining_space ? strlen(message) : remaining_space;
    strcpy(&core_1_buffer_[0] + core_1_buffer_ptr_, message);
    core_1_buffer_ptr_ += (int) print_len;
  }
}

void Console_Logger::flushBuffer() {
  if (xPortGetCoreID() == 0) {
    if (core_0_buffer_ptr_ != 0) {
      printIndent();
      if (core_0_has_name_) {
        printBeginning(core_0_log_type_, false);
        printName(&core_0_name_[0]);
      } else {
        printBeginning(core_0_log_type_, true);
      }
      Serial.println(core_0_buffer_);
    }
    core_0_has_name_ = false;
    core_0_buffer_ptr_ = 0;
    core_0_log_type_ = LOG_TYPE::NONE;
  } else {
    if (core_1_buffer_ptr_ != 0) {
      printIndent();
      if (core_1_has_name_) {
        printBeginning(core_1_log_type_, false);
        printName(&core_1_name_[0]);
      } else {
        printBeginning(core_1_log_type_, true);
      }
      Serial.println(core_1_buffer_);
    }
    core_1_has_name_ = false;
    core_1_buffer_ptr_ = 0;
    core_1_log_type_ = LOG_TYPE::NONE;
  }
}

void Console_Logger::print(const LOG_TYPE type, const char *name, const char *message) {
  if (!logging_active_)
    return;
  if (xPortGetCoreID() == 0) {
    if (core_0_buffer_ptr_ != 0) {
      flushBuffer();
      println(LOG_TYPE::ERR, "Logger Error: Buffer not flushed.");
    }
  } else {
    if (core_1_buffer_ptr_ != 0) {
      flushBuffer();
      println(LOG_TYPE::ERR, "Logger Error: Buffer not flushed.");
    }
  }
  setName(name);
  setLogType(type);
  addToBuffer(message);
}

void Console_Logger::print(const char *name, const char *message) {
  print(LOG_TYPE::INFO, name, message);
}

void Console_Logger::println(const char *name, const char *message) {
  println(LOG_TYPE::INFO, name, message);
}

void Console_Logger::println(const LOG_TYPE type, const char *name, const char *message) {
  if (!logging_active_)
    return;
  print(type, name, message);
  flushBuffer();
}

void Console_Logger::print(const char *message) {
  if (!logging_active_)
    return;
  print(LOG_TYPE::INFO, message);
}

void Console_Logger::print(const LOG_TYPE type, const char *message) {
  if (!logging_active_)
    return;
  setLogType(type);
  addToBuffer(message);
}

void Console_Logger::println(const char *message) {
  if (!logging_active_)
    return;
  println(LOG_TYPE::INFO, message);
}

void Console_Logger::println(const LOG_TYPE type, const char *message) {
  if (!logging_active_)
    return;
  print(type, message);
  flushBuffer();
}

void Console_Logger::add(const char *message) {
  addToBuffer(message);
}

void Console_Logger::add(const int message) {
  char buffer[LOGGER_T_BUFFER_LEN]{};
  sprintf(buffer, "%d", message);
  addToBuffer(buffer);
}

void Console_Logger::add(const unsigned long long message) {
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
    buffer[pointer] = char(int(buf % 10) + 48);
    buf = buf / 10;
    pointer--;
  }
  buffer[num_len - 1] = '\0';
  addToBuffer(buffer);
}

void Console_Logger::add(const float message) {
  char buffer[LOGGER_T_BUFFER_LEN]{};
  sprintf(buffer, "%f", message);
  addToBuffer(buffer);
}

void Console_Logger::add(const double message) {
  char buffer[LOGGER_T_BUFFER_LEN]{};
  sprintf(buffer, "%lf", message);
  addToBuffer(buffer);
}

void Console_Logger::add(const long message) {
  char buffer[LOGGER_T_BUFFER_LEN]{};
  sprintf(buffer, "%ld", message);
  addToBuffer(buffer);
}

void Console_Logger::add(const unsigned long message) {
  char buffer[LOGGER_T_BUFFER_LEN]{};
  sprintf(buffer, "%lu", message);
  addToBuffer(buffer);
}

void Console_Logger::add(const unsigned int message) {
  char buffer[LOGGER_T_BUFFER_LEN]{};
  sprintf(buffer, "%u", message);
  addToBuffer(buffer);
}

void Console_Logger::addln() {
  if (!logging_active_)
    return;
  flushBuffer();
}

Console_Logger logger;
