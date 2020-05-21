#ifndef __Code_Command__
#define __Code_Command__

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
  CodeCommand() :
      type_(UNKNOWN_C),
      code_(0),
      timestamp_(0) {};

  CodeCommand(const CodeType command_type, const unsigned long code, const unsigned long long code_timestamp) :
      type_(command_type),
      code_(code),
      timestamp_(code_timestamp) {};

  CodeType getType() const {
    return type_;
  }

  unsigned long getCode() const {
    return code_;
  }

  unsigned long long getTimestamp() const {
    return timestamp_;
  }
};

#endif // __Code_Command__