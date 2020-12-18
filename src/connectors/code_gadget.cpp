#include "code_gadget.h"

void Code_Gadget::setCommand(CodeType type, unsigned long new_command) {
  has_news_ = true;
  com_ = std::make_shared<CodeCommand>(type, new_command, system_timer.getTime());
}

Code_Gadget::Code_Gadget() :
  code_gadget_is_ready_(true),
  com_(nullptr),
  has_news_(false) {
};

bool Code_Gadget::codeGadgetIsReady() const {
  return code_gadget_is_ready_;
}

bool Code_Gadget::hasNewCommand() {
  bool buffer = has_news_;
  has_news_ = false;
  return buffer;
}

std::shared_ptr<CodeCommand> Code_Gadget::getCommand() {
  return com_;
}
