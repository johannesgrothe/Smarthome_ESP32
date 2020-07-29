

#include "code_remote.h"

// TODO: Durch C++-Standart-Datenstruktur ersetzen
CodeCommandBuffer::CodeCommandBuffer() :
    buffer_add_pointer_(0),
    buffer_get_pointer_(0) {}

void CodeCommandBuffer::addCode(CodeCommand *com) {
  bool pointers_differ = hasNewCode();
  if (code_list_[buffer_add_pointer_] != nullptr) {
    delete code_list_[buffer_add_pointer_];
  }
  code_list_[buffer_add_pointer_] = com;
  buffer_add_pointer_ = nextPos(buffer_add_pointer_);
  if (pointers_differ && !hasNewCode()) {
    buffer_get_pointer_++;
  }
}

// TODO: Check if there's any better way
bool CodeCommandBuffer::hasNewCode() const {
  return buffer_get_pointer_ != buffer_add_pointer_;
}

CodeCommand *CodeCommandBuffer::getCode() {
  if (!hasNewCode())
    return nullptr;
  CodeCommand *com = code_list_[buffer_get_pointer_];
  buffer_get_pointer_ = nextPos(buffer_get_pointer_);
  return com;
}

bool CodeCommandBuffer::codeIsDoubled(CodeCommand *com) const {
  for (byte k = 0; k < CODE_BUFFER_SIZE; k++) {
    if (code_list_[k] != nullptr) {
      if (code_list_[k]->getType() == com->getType() && code_list_[k]->getCode() == com->getCode()) {
        if (code_list_[k]->getTimestamp() + CODE_TIME_GAP > com->getTimestamp()) {
          return true;
        }
      }
    }
  }
  return false;
}

void CodeCommandBuffer::print() const {
  for (byte k = 0; k < CODE_BUFFER_SIZE; k++) {
    if (code_list_[k] != nullptr) {
      logger.print(LOG_TYPE::DATA, "");
      logger.add(k);
      logger.add(": ");
      logger.add(code_list_[k]->getTimestamp());
      logger.add(" -> ");
      logger.addln(code_list_[k]->getCode());
    } else {
      logger.print(LOG_TYPE::DATA, "");
      logger.add(k);
      logger.add(": ");
      logger.addln("null");
    }
  }
}

void CodeRemote::addCodeToBuffer(CodeCommand *code) {
  if (!codes.codeIsDoubled(code)) {
    codes.addCode(code);
    codes.print();
  } else {
    logger.println(LOG_TYPE::ERR, "Ignoring: Double Code");
  }
}

void CodeRemote::forwardCodeToGadgets(CodeCommand *code) const{
  logger.print("Forwarding Code to ");
  logger.add(gadgets.getGadgetCount());
  logger.addln(" Gadgets:");
  logger.incIndent();
  for (int i = 0; i < gadgets.getGadgetCount(); i++) {
    gadgets[i]->handleCodeUpdate(code->getCode());
  }
  logger.decIndent();
}

void CodeRemote::forwardAllCodes() {
  while (codes.hasNewCode()) {
    forwardCodeToGadgets(codes.getCode());
  }
}

CodeRemote::CodeRemote(JsonObject data) :
Remote(data){};

CodeRemote::CodeRemote(Request_Gadget *gadget, JsonObject data) :
Remote(gadget, data) {};

void CodeRemote::handleNewCodeFromGadget(CodeCommand *code) {
  addCodeToBuffer(code);
  sendCodeToRemote(code);
  forwardAllCodes();
}

void CodeRemote::handleNewCodeFromRequest(CodeCommand *code) {
  addCodeToBuffer(code);
  forwardAllCodes();
}