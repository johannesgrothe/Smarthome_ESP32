#include "code_gadget.h"

void Code_Gadget::setCommand(CodeType type, unsigned long new_command) {
  has_news_ = true;
  auto *buf_com(new CodeCommand(type, new_command, system_timer.getTime()));
  com_ = buf_com;
}

Code_Gadget::Code_Gadget() :
  code_gadget_is_ready_(false),
  com_(nullptr),
  has_news_(false) {
};

Code_Gadget::Code_Gadget(JsonObject data) :
  code_gadget_is_ready_(false),
  com_(nullptr),
  has_news_(false) {
  if (data.isNull()) {}
  code_gadget_is_ready_ = true;
};

bool Code_Gadget::codeGadgetIsReady() const {
  return code_gadget_is_ready_;
}

bool Code_Gadget::hasNewCommand() {
  bool buffer = has_news_;
  has_news_ = false;
  return buffer;
}

CodeCommand *Code_Gadget::getCommand() {
  return com_;
}
