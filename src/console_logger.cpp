#include "console_logger.h"

void Console_Logger::printOut(string str) {
  Serial.print(str.c_str());
}

void Console_Logger::printOut(char c) {
  stringstream ss;
  ss << c;
  printOut(ss.str());
}

void Console_Logger::println() {
  addToBuffer("\n");
}

void Console_Logger::printIndent() {
  stringstream sstr;
  sstr << xPortGetCoreID();
  if (xPortGetCoreID() == 0 || xPortGetCoreID() == 1) {
    printOut(sstr.str());
    printOut(" | ");
  } else {
    printOut("? | ");
  }

  byte local_indent;
  if (xPortGetCoreID() == 0) {
    local_indent = core_0_indent_;
  } else {
    local_indent = core_1_indent_;
  }

  for (byte k = 0; k < local_indent; k++) {
    for (byte j = 0; j < indent_len_; j++) {
      printOut(indent_char_);
    }
  }
}

void Console_Logger::printName(string name) {
  stringstream buf_str;
  buf_str << "[" << name << "] ";
  printOut(buf_str.str());
}

void Console_Logger::printBeginning(const LOG_TYPE type, const bool complete = true) {
  switch (type) {
    case LOG_TYPE::INFO:
      if (complete)
        printOut("[i] ");
      break;
    case LOG_TYPE::ERR:
      printOut("[x] ");
      break;
    case LOG_TYPE::DATA:
      printOut("-> ");
      break;
    case LOG_TYPE::NONE:
      break;
    case LOG_TYPE::FATAL:
      printOut("[:/] ");
    case LOG_TYPE::WARN:
      printOut("[!] ");
      break;
    default:
      break;
  }
  print(" ");
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

void Console_Logger::setName(const string& name) {
  if (xPortGetCoreID() == 0) {
    core_0_name_ = name;
    core_0_has_name_ = true;
  } else {
    core_1_name_ = name;
    core_1_has_name_ = true;
  }
}

void Console_Logger::setLogType(const LOG_TYPE type) {
  if (xPortGetCoreID() == 0) {
    core_0_log_type_ = type;
  } else {
    core_1_log_type_ = type;
  }
}

//void Console_Logger::addToBuffer(const char *message) {
//  if (xPortGetCoreID() == 0) {
//    unsigned int remaining_space = ((LOGGER_MAX_BUFFER_LEN - core_0_buffer_ptr_) - 2);
//    unsigned int print_len = strlen(message) < remaining_space ? strlen(message) : remaining_space;
//    strcpy(&core_0_buffer_[0] + core_0_buffer_ptr_, message);
//    core_0_buffer_ptr_ += print_len;
//  } else {
//    unsigned int remaining_space = ((LOGGER_MAX_BUFFER_LEN - core_1_buffer_ptr_) - 2);
//    unsigned int print_len = strlen(message) < remaining_space ? strlen(message) : remaining_space;
//    strcpy(&core_1_buffer_[0] + core_1_buffer_ptr_, message);
//    core_1_buffer_ptr_ += (int) print_len;
//  }
//}

void Console_Logger::addToBuffer(string s) {
  if (xPortGetCoreID() == 0) {
    for(char c:s) {
      if (c != '\n')
        core_0_buffer_ << c;
    }
  } else {
    for(char c:s) {
      if (c != '\n')
        core_1_buffer_ << c;
    }
  }
}

void Console_Logger::flushBuffer(){
  if (xPortGetCoreID() == 0) {
    printIndent();
    printBeginning(core_0_log_type_);
    if (core_0_has_name_) {
      printName(core_0_name_);
      core_0_has_name_ = false;
    }
    printOut(core_0_buffer_.str());
    printOut('\n');
    core_0_buffer_.str(string());
  } else {
    printIndent();
    printBeginning(core_1_log_type_);
    if (core_1_has_name_) {
      printName(core_1_name_);
      core_1_has_name_ = false;
    }
    printOut(core_1_buffer_.str());
    printOut('\n');
    core_1_buffer_.str(string());
  }
}

Console_Logger logger;
