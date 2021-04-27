#include "console_logger.h"

void Console_Logger::printOut(string str) {
  #ifndef ARDUINO
  std::cout << str;
  #else
  Serial.print(str.c_str());
  #endif
}

Console_Logger::Console_Logger() :
    logger_states_(),
    indent_char_(' '),
    logging_active_(LOGGING_ACTIVE) {
  for (int i = 0; i < LOGGER_THREAD_COUNT; i++) {
    logger_states_[i] = std::make_shared<LoggerState>();
  }
};

void Console_Logger::activateLogging() {
  logging_active_ = true;
}

void Console_Logger::deactivateLogging() {
  logging_active_ = false;
}

void Console_Logger::setCallback(std::function<void(LOG_TYPE, string, string, int)> new_callback) {
  callback_ = move(new_callback);
}

uint32_t Console_Logger::getTaskID() {
  #ifndef ARDUINO
  return 0;
  #else
  return xPortGetCoreID();
  #endif
}

Console_Logger &Console_Logger::operator++() noexcept {
  auto thread = getTaskID();
  auto state = logger_states_[thread];
  state->incIndent();
  return *this;
}

Console_Logger &Console_Logger::operator--() noexcept {
  auto thread = getTaskID();
  auto state = logger_states_[thread];
  state->decIndent();
  return *this;
}

std::string Console_Logger::logLvlToString(LOG_TYPE loglevel) {
  switch (loglevel) {
    case LOG_TYPE::INFO:
      return "i";
      break;
    case LOG_TYPE::ERR:
      return "x";
      break;
    case LOG_TYPE::DATA:
      return "->";
      break;
    case LOG_TYPE::NONE:
      break;
    case LOG_TYPE::FATAL:
      return ":/";
      break;
    case LOG_TYPE::WARN:
      return "!";
      break;
    default:
      break;
  }
  return "?!";
}

std::string Console_Logger::getIndentationStr(uint8_t indentation) const {
  std::stringstream out_str;
  for (uint8_t i = 0; i < indentation; i++) {
    for (uint8_t j = 0; j < INDENT_LEN; j++) {
      out_str << indent_char_;
    }
  }
  return out_str.str();
}

void Console_Logger::flushData(std::shared_ptr<LoggerState> data, uint32_t task_id) {
  std::stringstream out_stream;
  out_stream << task_id
             << " | "
             << getIndentationStr(data->getIndent())
             << "["
             << logLvlToString(data->getLevel())
             << "]";
  if (data->getSender() != "") {
    out_stream << " (" << data->getSender() << ")";
  }
  out_stream << ": " << data->getData();
  printOut(out_stream.str());
}

Console_Logger &Console_Logger::level(LOG_TYPE log_lvl) {
  auto thread = getTaskID();
  auto state = logger_states_[thread];
  state->setLevel(log_lvl);
  return *this;
}

Console_Logger &Console_Logger::sender(std::string name) {
  auto thread = getTaskID();
  auto state = logger_states_[thread];
  state->setSender(name);
  return *this;
}

Console_Logger &Console_Logger::log(std::string name, LOG_TYPE log_lvl) {
  auto thread = getTaskID();
  auto state = logger_states_[thread];
  state->setSender(name);
  state->setLevel(log_lvl);
  return *this;
}

Console_Logger &Console_Logger::log(std::string name) {
  return sender(name);
}

Console_Logger &Console_Logger::log(LOG_TYPE log_lvl) {
  return level(log_lvl);
}

LoggerState::LoggerState() :
    loglevel_(LOG_TYPE::INFO),
    sender_(),
    indentation_(0),
    data_() {
  reset();
}

void LoggerState::reset() {
  resetData();
  resetLevel();
  resetSender();
}

std::string LoggerState::getData() const {
  return data_.str();
}

void LoggerState::resetData() {
  data_ = std::stringstream();
}

std::string LoggerState::getSender() const {
  return sender_;
}

void LoggerState::setSender(std::string sender) {
  sender_ = sender;
}

LOG_TYPE LoggerState::getLevel() {
  return loglevel_;
}

void LoggerState::setLevel(LOG_TYPE loglevel) {
  loglevel_ = loglevel;
}

void LoggerState::incIndent() {
  indentation_++;
}

void LoggerState::decIndent() {
  if (indentation_ > 0) {
    indentation_--;
  }
}

void LoggerState::setIndent(uint8_t indentation) {
  indentation_ = indentation;
}

uint8_t LoggerState::getIndent() const {
  return indentation_;
}

void LoggerState::resetLevel() {
  loglevel_ = LOG_TYPE::INFO;
}

void LoggerState::resetSender() {
  sender_ = "";
}

Console_Logger logger;
