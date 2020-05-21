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
  CodeType type;
  unsigned long code;
  unsigned long long timestamp;

public:
  CodeCommand() :
      type(UNKNOWN_C),
      code(0),
      timestamp(0) {};

  CodeCommand(CodeType command_type, unsigned long code_type, unsigned long long code_timestamp) :
      type(command_type),
      code(code_type),
      timestamp(code_timestamp) {};

  CodeType getType() {
    return type;
  }

  unsigned long getCode() {
    return code;
  }

  unsigned long long getTimestamp() {
    return timestamp;
  }
};

#endif // __Code_Command__