#include "ir_gadget.h"

IR_Gadget::IR_Gadget() :
  Code_Gadget(),
  receiver_(nullptr),
  blaster_(nullptr) {
    logger.println("No IR Configured");
    code_gadget_is_ready_ = false;
  };

IR_Gadget::IR_Gadget(JsonObject data) :
    Code_Gadget(data) {
    bool everything_ok = true;
    if (data["recv_pin"] != nullptr) {
      uint8_t r_pin = data["recv_pin"].as<int>();
      receiver_ = new IRrecv(r_pin);
      receiver_->enableIRIn();
      logger.print(LOG_TYPE::DATA, "Receiver-Pin: ");
      logger.addln(r_pin);
    } else {
      everything_ok = false;
      logger.println(LOG_TYPE::ERR, "'recv_pin' nicht spezifiziert.");
    }
    if (data["send_pin"] != nullptr) {
      uint8_t b_pin = data["send_pin"].as<int>();
      blaster_ = new IRsend(b_pin);
      blaster_->begin();
      logger.print(LOG_TYPE::DATA, "Blaster-Pin: ");
      logger.addln(b_pin);
    } else {
      everything_ok = false;
      logger.println(LOG_TYPE::ERR, "'send_pin' nicht spezifiziert.");
    }
    code_gadget_is_ready_ = everything_ok;
  };

  void IR_Gadget::refresh() {
    if (!code_gadget_is_ready_) {
      return;
    }
    decode_results results{};
    if (receiver_->decode(&results)) {
      unsigned long received_command{};
      int8_t command_type{};
      command_type = results.decode_type;
      received_command = results.value;
      receiver_->resume();
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
  }

  bool IR_Gadget::sendRawIR(const uint16_t raw_data[], const uint8_t content_length) {
    if (!code_gadget_is_ready_) {
      return false;
    }
    logger.print("System / IR", "Sending Raw Command, 38kHz, ");
    blaster_->sendRaw(raw_data, content_length, 38);
    logger.add(content_length);
    logger.addln(" Blocks.");
    receiver_->resume();
    return true;
  }

  bool IR_Gadget::sendIR(unsigned long command, const uint8_t com_type) {
    if (!code_gadget_is_ready_) {
      return false;
    }
    logger.print("System / IR", "Sending: ");
    switch (com_type) {
      case NEC:
        blaster_->sendNEC(command);
        break;
      case SONY:
        blaster_->sendSony(command);
        break;
      case SAMSUNG:
        blaster_->sendSAMSUNG(command);
        break;
      case LG:
        blaster_->sendLG(command);
        break;
      case DENON:
        blaster_->sendDenon(command);
        break;
      default:
        logger.addln("Unsupported Command.");
        return false;
    }
    logger.addln(command);
    receiver_->resume();
    return true;
  }
