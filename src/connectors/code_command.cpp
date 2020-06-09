#include "code_command.h"

CodeCommand::CodeCommand() :
  type_(UNKNOWN_C),
  code_(0),
  timestamp_(0) {};

CodeCommand::CodeCommand(const CodeType command_type, const unsigned long code, const unsigned long long code_timestamp)
  :
  type_(command_type),
  code_(code),
  timestamp_(code_timestamp) {};

CodeType CodeCommand::getType() const {
  return type_;
}

unsigned long CodeCommand::getCode() const {
  return code_;
}

unsigned long long CodeCommand::getTimestamp() const {
  return timestamp_;
}
