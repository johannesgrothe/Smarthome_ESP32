#include "code_command_buffer.h"

#include <utility>

CodeCommandBuffer::CodeCommandBuffer() :
    new_codes_(),
    code_db_() {}

bool CodeCommandBuffer::addCode(const std::shared_ptr<CodeCommand>& com) {
  if (addCodeToDb(com)) {
    new_codes_.push_back(com);
    return true;
  }
  return false;
}

bool CodeCommandBuffer::hasNewCode() const {
  return !new_codes_.empty();
}

std::shared_ptr<CodeCommand> CodeCommandBuffer::getCode() {
  //TODO: Threw exception, check
  if (!hasNewCode())
    return nullptr;
  auto buf_code = new_codes_[new_codes_.size()];
  new_codes_.pop_back();
  return buf_code;
}

std::string CodeCommandBuffer::toString() const {
  std::stringstream sstr;
  int i = 0;
  for (const auto& code: code_db_) {
    sstr << "[" << i << "] " << code->getTimestamp()
         << " -> " << codeTypeToString(code->getType())
         << "-" << code->getCode() << std::endl;

    i ++;
  }
  return sstr.str();
}

bool CodeCommandBuffer::addCodeToDb(const std::shared_ptr<CodeCommand> &code) {
  int counter = 0;
  for (const auto &saved_code: code_db_) {
    if (saved_code->getCode() == code->getCode() && saved_code->getType() == code->getType()) {
      if ((saved_code->getTimestamp() + 100) < code->getTimestamp()) {
        // Code is interpreted as "the same"
        code_db_.at(counter) = code;
        return false;
      }
      code_db_.at(counter) = code;
      return true;
    }
    counter++;
  }
  code_db_.push_back(code);
  return true;
}
