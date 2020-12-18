#pragma once
#include "code_command.h"
#include "console_logger.h"
#include "system_settings.h"
#include <utility>
#include <Wire.h>
#include <memory>

static CodeType stringToCodeType(const char *input) {
    if (strcmp(input, "SERIAL") == 0) {
        return SERIAL_C;
    }
    else if (strcmp(input, "RADIO") == 0) {
        return RADIO_C;
    }
    else if (strcmp(input, "IR_UNKNOWN") == 0) {
        return IR_UNKNOWN_C;
    }
    else if (strcmp(input, "IR_NEC") == 0) {
        return IR_NEC_C;
    }
    else if (strcmp(input, "IR_SONY") == 0) {
        return IR_SONY_C;
    }
    else if (strcmp(input, "IR_RC5") == 0) {
        return IR_RC5_C;
    }
    else if (strcmp(input, "IR_RC6") == 0) {
        return IR_RC6_C;
    }
    else if (strcmp(input, "IR_SAMSUNG") == 0) {
        return IR_SAMSUNG_C;
    }
    else if (strcmp(input, "IR_LG") == 0) {
        return IR_LG_C;
    }
    else if (strcmp(input, "IR_PANASONIC") == 0) {
        return IR_PANASONIC_C;
    }
    else if (strcmp(input, "IR_DENON") == 0) {
        return IR_DENON_C;
    }
    else {
        return UNKNOWN_C;
    }
};

static void codeTypeToString(CodeType code, char *buf) {
    switch (code) {
        case UNKNOWN_C:
            strncpy(buf, "UNKNOWN", CODE_TYPE_NAME_LEN);
            break;
        case SERIAL_C:
            strncpy(buf, "SERIAL", CODE_TYPE_NAME_LEN);
            break;
        case RADIO_C:
            strncpy(buf, "RADIO", CODE_TYPE_NAME_LEN);
            break;
        case IR_UNKNOWN_C:
            strncpy(buf, "IR_UNKNOWN", CODE_TYPE_NAME_LEN);
            break;
        case IR_NEC_C:
            strncpy(buf, "IR_NEC", CODE_TYPE_NAME_LEN);
            break;
        case IR_SONY_C:
            strncpy(buf, "IR_SONY", CODE_TYPE_NAME_LEN);
            break;
        case IR_RC5_C:
            strncpy(buf, "IR_RC5", CODE_TYPE_NAME_LEN);
            break;
        case IR_RC6_C:
            strncpy(buf, "IR_RC6", CODE_TYPE_NAME_LEN);
            break;
        case IR_SAMSUNG_C:
            strncpy(buf, "IR_SAMSUNG", CODE_TYPE_NAME_LEN);
            break;
        case IR_LG_C:
            strncpy(buf, "IR_LG", CODE_TYPE_NAME_LEN);
            break;
        case IR_PANASONIC_C:
            strncpy(buf, "IR_PANASONIC", CODE_TYPE_NAME_LEN);
            break;
        case IR_DENON_C:
            strncpy(buf, "IR_DENON", CODE_TYPE_NAME_LEN);
            break;
        default:
            strncpy(buf, "UNDEFINED", CODE_TYPE_NAME_LEN);
            break;
    }
}

/**
 * A buffer for code commands
 */
class CodeCommandBuffer {
private:

    std::shared_ptr<CodeCommand> code_list_[CODE_BUFFER_SIZE];
    byte buffer_add_pointer_{};
    byte buffer_get_pointer_{};

    static byte nextPos(byte start) {
        start++;
        if (start == CODE_BUFFER_SIZE)
            start = 0;
        return start;
    }

public:
    CodeCommandBuffer();

    void addCode(std::shared_ptr<CodeCommand>);

    bool hasNewCode() const;

    std::shared_ptr<CodeCommand> getCode();

    bool codeIsDoubled(std::shared_ptr<CodeCommand> com) const;

    void print() const;
};