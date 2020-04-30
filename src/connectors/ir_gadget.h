#ifndef __IR_Connector__
#define __IR_Connector__

#include <Arduino.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <ArduinoJson.h>
#include "code_gadget.h"

// Gadget to send and receive IR-Commands
class IR_Gadget : public Code_Gadget {
protected:

  IRrecv *receiver;
  IRsend *blaster;

public:

  IR_Gadget() :
    Code_Gadget(),
    receiver(nullptr),
    blaster(nullptr) {
  };

  explicit IR_Gadget(JsonObject data) :
    Code_Gadget(data) {
    logger.println("Creating IR-Gadget");
    logger.incIndent();
    bool everything_ok = true;
    if (data["recv_pin"] != nullptr) {
      uint8_t r_pin = data["recv_pin"].as<int>();
      receiver = new IRrecv(r_pin);
      receiver->enableIRIn();
      logger.print(LOG_DATA, "Receiver-Pin: ");
      logger.addln(r_pin);
    } else {
      everything_ok = false;
      logger.println(LOG_ERR, "'recv_pin' nicht spezifiziert.");
    }
    if (data["send_pin"] != nullptr) {
      uint8_t b_pin = data["send_pin"].as<int>();
      blaster = new IRsend(b_pin);
      blaster->begin();
      logger.print(LOG_DATA, "Blaster-Pin: ");
      logger.addln(b_pin);
    } else {
      everything_ok = false;
      logger.println(LOG_ERR, "'send_pin' nicht spezifiziert.");
    }
    code_gadget_is_ready = everything_ok;
    logger.decIndent();
  };

  void refresh() override {
    if (!code_gadget_is_ready) {
      return;
    }
    decode_results results{};
    if (receiver->decode(&results)) {
      unsigned long received_command{};
      int8_t command_type{};
      command_type = results.decode_type;
      received_command = results.value;
      receiver->resume();
      if (received_command != 0xFFFFFFFF) {
        switch (command_type) {
          default:
          case UNKNOWN:
            setCommand(IR_UNKNOWN_C, received_command);
            break;
          case NEC:
            setCommand(IR_NEC_C, received_command);
            break;
          case SONY:
            setCommand(IR_SONY_C, received_command);
            break;
          case RC5:
            setCommand(IR_RC5_C, received_command);
            break;
          case RC6:
            setCommand(IR_RC6_C, received_command);
            break;
          case DISH:
            setCommand(IR_UNKNOWN_C, received_command);
            break;
          case SHARP:
            setCommand(IR_UNKNOWN_C, received_command);
            break;
          case JVC:
            setCommand(IR_UNKNOWN_C, received_command);
            break;
          case SANYO:
            setCommand(IR_UNKNOWN_C, received_command);
            break;
          case MITSUBISHI:
            setCommand(IR_UNKNOWN_C, received_command);
            break;
          case SAMSUNG:
            setCommand(IR_SAMSUNG_C, received_command);
            break;
          case LG:
            setCommand(IR_LG_C, received_command);
            break;
          case WHYNTER:
            setCommand(IR_UNKNOWN_C, received_command);
            break;
          case AIWA_RC_T501:
            setCommand(IR_UNKNOWN_C, received_command);
            break;
          case PANASONIC:
            setCommand(IR_PANASONIC_C, received_command);
            break;
          case DENON:
            setCommand(IR_DENON_C, received_command);
            break;
        }
      }
    }
  };

  bool sendRawIR(uint16_t raw_data[], uint8_t content_length) {
    logger.print("System / IR", "Sending Raw Command, 38kHz, ");
    blaster->sendRaw(raw_data, content_length, 38);
    logger.add(content_length);
    logger.addln(" Blocks.");
    receiver->resume();
    return true;
  }

  bool sendIR(unsigned long command, uint8_t com_type) {
    logger.print("System / IR", "Sending: ");
    switch (com_type) {
      case NEC:
        blaster->sendNEC(command);
        break;
      case SONY:
        blaster->sendSony(command);
        break;
      case SAMSUNG:
        blaster->sendSAMSUNG(command);
        break;
      case LG:
        blaster->sendLG(command);
        break;
      case DENON:
        blaster->sendDenon(command);
        break;
      default:
        logger.addln("Unsupported Command.");
        return false;
    }
    logger.add("0x");
    logger.addln(command, HEX);
    receiver->resume();
    return true;
  };
};

#endif