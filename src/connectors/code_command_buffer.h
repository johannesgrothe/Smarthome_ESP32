#pragma once

#include "code_command.h"
#include "console_logger.h"
#include "system_settings.h"
#include <utility>
#include <Wire.h>
#include <memory>

static std::string codeTypeToString(CodeType code) {
  switch (code) {
    case UNKNOWN_C:
      return "UNKNOWN";
    case SERIAL_C:
      return "SERIAL";
    case RADIO_C:
      return "RADIO";
    case IR_UNKNOWN_C:
      return "IR_UNKNOWN";
    case IR_NEC_C:
      return "IR_NEC";
    case IR_SONY_C:
      return "IR_SONY";
    case IR_RC5_C:
      return "IR_RC5";
    case IR_RC6_C:
      return "IR_RC6";
    case IR_SAMSUNG_C:
      return "IR_SAMSUNG";
    case IR_LG_C:
      return "IR_LG";
    case IR_PANASONIC_C:
      return "IR_PANASONIC";
    case IR_DENON_C:
      return "IR_DENON";
    default:
      return "UNDEFINED";
  }
}

/**
 * A buffer for code commands
 */
class CodeCommandBuffer {
private:

  std::vector<std::shared_ptr<CodeCommand>> new_codes_;
  std::vector<std::shared_ptr<CodeCommand>> code_db_;

  bool addCodeToDb(const std::shared_ptr<CodeCommand>&);

public:
  CodeCommandBuffer();

  bool addCode(const std::shared_ptr<CodeCommand>&);

  bool hasNewCode() const;

  std::shared_ptr<CodeCommand> getCode();

  std::string toString() const;
};