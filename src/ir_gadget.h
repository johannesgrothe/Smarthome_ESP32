#ifndef __IR_Gadget__
#define __IR_Gadget__

#define SEND_PIN 13

// #include <Arduino.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>

//Gadget for IR Usage

class IR_Gadget
{
  protected:

    uint8_t recvPin;
    IRrecv receiver;
    IRsend blaster;
    long received_command{};
    uint8_t command_type{};
    bool has_new_command{};

  public:

    IR_Gadget():
      recvPin(0),
      receiver(IRrecv(0)),
      blaster(IRsend(0))
    {
    };

    IR_Gadget(uint8_t receiver_pin, uint8_t blaster_pin):
      recvPin(receiver_pin),
      receiver(IRrecv(receiver_pin)),
      blaster(IRsend(blaster_pin))
      {
      };

    void refresh()
    {
      decode_results results;
      if (receiver.decode(&results))
      {
        has_new_command = true;
        command_type = results.decode_type;
        received_command = results.value;
        receiver.resume();

        // Serial Output
        Serial.println("[System / IR Receiver] Received Something:\n  Encoding: ");
        switch (command_type)
        {
          default:
          case UNKNOWN:      Serial.print("UNKNOWN");       break ;
          case NEC:          Serial.print("NEC");           break ;
          case SONY:         Serial.print("SONY");          break ;
          case RC5:          Serial.print("RC5");           break ;
          case RC6:          Serial.print("RC6");           break ;
          case DISH:         Serial.print("DISH");          break ;
          case SHARP:        Serial.print("SHARP");         break ;
          case JVC:          Serial.print("JVC");           break ;
          case SANYO:        Serial.print("SANYO");         break ;
          case MITSUBISHI:   Serial.print("MITSUBISHI");    break ;
          case SAMSUNG:      Serial.print("SAMSUNG");       break ;
          case LG:           Serial.print("LG");            break ;
          case WHYNTER:      Serial.print("WHYNTER");       break ;
          case AIWA_RC_T501: Serial.print("AIWA_RC_T501");  break ;
          case PANASONIC:    Serial.print("PANASONIC");     break ;
          case DENON:        Serial.print("Denon");         break ;
        }
        Serial.print("\n  Command: ");
        Serial.print(received_command, HEX);
        Serial.println("");
      }
    };

    bool sendIR(long command, uint8_t command_type)
    {
      Serial.print("[System / IR Receiver] Sending:\n");
      switch (command_type)
      {
        case NEC:
          blaster.sendNEC(command);
          break;
        case SONY:
          blaster.sendSony(command);
          break;
        case SAMSUNG:
          blaster.sendSAMSUNG(command);
          break;
        case LG:
          blaster.sendLG(command);
          break;
        case DENON:
          blaster.sendDenon(command);
          break;
        default:
          Serial.print(" Unsupported Command.\n");
      }
      receiver.resume();
    };

    bool init()
    {
      receiver.enableIRIn();
    };

    bool hasReceived()
    {
      bool buf = has_new_command;
      has_new_command = false;
      return buf;
    };

    long getCommand()
    {
      return received_command;
    }

    uint8_t getCommandEncoding()
    {
      return command_type;
    }
};

#endif