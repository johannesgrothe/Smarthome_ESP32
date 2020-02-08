#ifndef __IR_Connector__
#define __IR_Connector__

#include <Arduino.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <ArduinoJson.h>

// Gadget to send and receive IR-Commands
class IR_Gadget {
protected:

  IRrecv *receiver;
  IRsend *blaster;
  long received_command{};
  int8_t command_type{};
  bool has_new_command{};
  bool is_initialized;
  bool is_ready{};

public:

  IR_Gadget() :
    receiver(nullptr),
    blaster(nullptr),
    is_initialized(false) {
  };

  IR_Gadget(uint8_t receiver_pin, uint8_t blaster_pin) :
    receiver(new IRrecv(receiver_pin)),
    blaster(new IRsend(blaster_pin)),
    is_initialized(true) {
  };

  explicit IR_Gadget(JsonObject data) {
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
    is_initialized = everything_ok;
  };

  void refresh() {
    if (!is_initialized) {
      return;
    }
    decode_results results{};
    if (receiver->decode(&results)) {
      has_new_command = true;
      command_type = results.decode_type;
      received_command = results.value;
      receiver->resume();

      // Serial Output
      Serial.println("[System / IR Receiver] Received Something:\n  Encoding: ");
      switch (command_type) {
        default:
        case UNKNOWN:
          Serial.print("UNKNOWN");
          break;
        case NEC:
          Serial.print("NEC");
          break;
        case SONY:
          Serial.print("SONY");
          break;
        case RC5:
          Serial.print("RC5");
          break;
        case RC6:
          Serial.print("RC6");
          break;
        case DISH:
          Serial.print("DISH");
          break;
        case SHARP:
          Serial.print("SHARP");
          break;
        case JVC:
          Serial.print("JVC");
          break;
        case SANYO:
          Serial.print("SANYO");
          break;
        case MITSUBISHI:
          Serial.print("MITSUBISHI");
          break;
        case SAMSUNG:
          Serial.print("SAMSUNG");
          break;
        case LG:
          Serial.print("LG");
          break;
        case WHYNTER:
          Serial.print("WHYNTER");
          break;
        case AIWA_RC_T501:
          Serial.print("AIWA_RC_T501");
          break;
        case PANASONIC:
          Serial.print("PANASONIC");
          break;
        case DENON:
          Serial.print("Denon");
          break;
      }
      Serial.print("\n  Command: ");
      Serial.print(received_command, HEX);
      Serial.println("");
    }
  };

  bool sendIR(long command, uint8_t command_type) {
    Serial.print("[System / IR Receiver] Sending:\n");
    switch (command_type) {
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

  bool init() {
//    receiver->enableIRIn();
//    receiver->resume();
    return true;
  };

  bool hasReceived() {
    bool buf = has_new_command;
    has_new_command = false;
    return buf;
  };

  long getCommand() {
    return received_command;
  }

  uint8_t getCommandEncoding() {
    return command_type;
  }

  bool isInitialized() {
    return is_initialized;
  }
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