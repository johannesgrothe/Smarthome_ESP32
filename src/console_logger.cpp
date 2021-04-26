#include "console_logger.h"

#include <utility>

void Console_Logger::printOut(string str) {
  #ifdef UNIT_TEST
  std::cout << str;
  #else
  Serial.print(str.c_str());
  #endif
}

void Console_Logger::printOut(char c) {
  stringstream ss;
  ss << c;
  printOut(ss.str());
}

void Console_Logger::printIndent() {
  stringstream sstr;
  sstr << getTaskID();
  if (getTaskID() == 0 || getTaskID() == 1) {
    printOut(sstr.str());
    printOut(" | ");
  } else {
    printOut("? | ");
  }

  uint8_t local_indent;
  if (getTaskID() == 0) {
    local_indent = core_0_indent_;
  } else {
    local_indent = core_1_indent_;
  }

  for (uint8_t k = 0; k < local_indent; k++) {
    for (uint8_t j = 0; j < indent_len_; j++) {
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

void Console_Logger::printnl() {
  flushBuffer();
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
  if (getTaskID() == 0) {
    core_0_indent_++;
  } else {
    core_1_indent_++;
  }
};

void Console_Logger::decIndent() {
  if (getTaskID() == 0) {
    if (core_0_indent_ > 0)
      core_0_indent_--;
  } else {
    if (core_1_indent_ > 0)
      core_1_indent_--;
  }
};

void Console_Logger::setName(const string& name) {
  if (getTaskID() == 0) {
    core_0_name_ = name;
    core_0_has_name_ = true;
  } else {
    core_1_name_ = name;
    core_1_has_name_ = true;
  }
}

void Console_Logger::setLogType(const LOG_TYPE type) {
  if (getTaskID() == 0) {
    core_0_log_type_ = type;
  } else {
    core_1_log_type_ = type;
  }
}

void Console_Logger::setCallback(std::function<void(LOG_TYPE ,string ,string ,int )> new_callback){
  callback_ = move(new_callback);
}

void Console_Logger::setCallbackStatus(LOG_TYPE type, bool status) {
  callback_status_[(int )type] = status;
}

void Console_Logger::callCallback(LOG_TYPE type, string message, string name, int task_id){
  if (callback_status_[(int) type]) {
    callback_(type, move(message), move(name), task_id);
  }
}


void Console_Logger::addToBuffer(string s) {
  if (getTaskID() == 0) {
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
  if (getTaskID() == 0) {
    if(core_0_buffer_.str().size() == 0) return;
    printIndent();
    printBeginning(core_0_log_type_);
    if (core_0_has_name_) {
      printName(core_0_name_);
      core_0_has_name_ = false;
    }
    printOut(core_0_buffer_.str());
    printOut('\n');
    callCallback(core_0_log_type_, core_0_buffer_.str(), core_0_name_, 0);
    core_0_buffer_.str(string());
    setLogType(LOG_TYPE::INFO);
  } else {
    if(core_1_buffer_.str().size() == 0) return;
    printIndent();
    printBeginning(core_1_log_type_);
    if (core_1_has_name_) {
      printName(core_1_name_);
      core_1_has_name_ = false;
    }
    printOut(core_1_buffer_.str());
    printOut('\n');
    callCallback(core_1_log_type_, core_1_buffer_.str(), core_1_name_, 1);
    core_1_buffer_.str(string());
    setLogType(LOG_TYPE::INFO);
  }
}

uint32_t Console_Logger::getTaskID() {
  #ifdef UNIT_TEST
  return 0;
  #else
  return xPortGetCoreID();
  #endif
}

Console_Logger logger;
