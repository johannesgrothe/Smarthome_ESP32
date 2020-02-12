#ifndef __IR_Connector__
#define __IR_Connector__

#include <Arduino.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <ArduinoJson.h>
#include "code_connector.h"

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
    Serial.println("   [INFO] Creating IR-Gadget");
    bool everything_ok = true;
    if (data["recv_pin"] != nullptr) {
      uint8_t r_pin = data["recv_pin"].as<int>();
      receiver = new IRrecv(r_pin);
      Serial.printf("     => Receiver-Pin: %d\n", r_pin);
    } else {
      everything_ok = false;
      Serial.println("     => [ERR] \"recv_pin\" nicht spezifiziert.");
    }
    if (data["send_pin"] != nullptr) {
      uint8_t b_pin = data["send_pin"].as<int>();
      blaster = new IRsend(b_pin);
      Serial.printf("     => Blaster-Pin: %d\n", b_pin);
    } else {
      everything_ok = false;
      Serial.println("     => [ERR] \"send_pin\" nicht spezifiziert.");
    }
//    Serial.println(receiver->getTolerance());
    receiver->enableIRIn();
//    receiver->resume();
    is_initialized = everything_ok;
  };

  void refresh() override {
    if (!is_initialized) {
      return;
    }
    decode_results results{};
    if (receiver->decode(&results)) {
      unsigned long received_command{};
      int8_t command_type{};
      command_type = results.decode_type;
      received_command = results.value;
      receiver->resume();

//      Serial.println("[System / IR Receiver] Received Something:\n  Encoding: ");
//      switch (command_type) {
//        default:
//        case UNKNOWN:
//          Serial.print("UNKNOWN");
//          break;
//        case NEC:
//          Serial.print("NEC");
//          break;
//        case SONY:
//          Serial.print("SONY");
//          break;
//        case RC5:
//          Serial.print("RC5");
//          break;
//        case RC6:
//          Serial.print("RC6");
//          break;
//        case DISH:
//          Serial.print("DISH");
//          break;
//        case SHARP:
//          Serial.print("SHARP");
//          break;
//        case JVC:
//          Serial.print("JVC");
//          break;
//        case SANYO:
//          Serial.print("SANYO");
//          break;
//        case MITSUBISHI:
//          Serial.print("MITSUBISHI");
//          break;
//        case SAMSUNG:
//          Serial.print("SAMSUNG");
//          break;
//        case LG:
//          Serial.print("LG");
//          break;
//        case WHYNTER:
//          Serial.print("WHYNTER");
//          break;
//        case AIWA_RC_T501:
//          Serial.print("AIWA_RC_T501");
//          break;
//        case PANASONIC:
//          Serial.print("PANASONIC");
//          break;
//        case DENON:
//          Serial.print("Denon");
//          break;
//      }
      if (command_type != UNKNOWN) {
        last_command_hex = received_command;
        last_command_str[0] = '\0';
        has_news = true;
      }
    }
  };

  bool sendIR(long command, uint8_t com_type) {
    Serial.print("[System / IR Receiver] Sending:\n");
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
        Serial.print(" Unsupported Command.\n");
    }
    receiver->resume();
    return true;
  };

//  uint8_t getCommandEncoding() {
//    return command_type;
//  }
};


// Connector for IR Usage
class IR_Connector {
protected:

  IR_Gadget *irgadget;

  bool initialized_serial;

  // DynamicJsonDocument recv_commands;

  bool decode_ir() {
    Serial.println("[WARN] decode_ir() not implemented");
    return false;
  };

  bool send_ir() {
    Serial.println("[WARN] send_ir() not implemented");
    return false;
  };

public:

  IR_Connector() :
    irgadget(nullptr),
    initialized_serial(false) {}

  void init_ir_con(IR_Gadget *new_ir_gadget) {
    initialized_serial = true;
    irgadget = new_ir_gadget;
  }

};

#endif