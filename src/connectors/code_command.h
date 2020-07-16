#pragma once

enum CodeType {
  UNKNOWN_C,
  SERIAL_C,
  RADIO_C,
  IR_UNKNOWN_C,
  IR_NEC_C,
  IR_SONY_C,
  IR_RC5_C,
  IR_RC6_C,
  IR_SAMSUNG_C,
  IR_LG_C,
  IR_PANASONIC_C,
  IR_DENON_C
};

class CodeCommand {
private:
  const CodeType type_;
  const unsigned long code_;
  const unsigned long long timestamp_;

public:
  CodeCommand();

  CodeCommand(CodeType, unsigned long, unsigned long long);

  CodeType getType() const;

  unsigned long getCode() const;

  unsigned long long getTimestamp() const;
};
