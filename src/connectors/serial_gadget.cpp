#include "serial_gadget.h"
#include <sstream>

void Serial_Gadget::executeRequestSending(Request *req) {
  Serial.printf("!r_p[%s]_b[%s]_\n", req->getPath().c_str(), req->getBody().c_str());
}

Serial_Gadget::Serial_Gadget() :
  Code_Gadget(),
  Request_Gadget() {};

Serial_Gadget::Serial_Gadget(JsonObject data) :
  Code_Gadget(data),
  Request_Gadget(RequestGadgetType::SERIAL_G, data) {
  if (data.isNull()) {}
  logger.println("Creating Serial Gadget");
  logger.incIndent();
  logger.println(LOG_TYPE::DATA, "Using default Serial Connection");
  logger.decIndent();
}

void Serial_Gadget::refresh() {
  if (!code_gadget_is_ready_ || !request_gadget_is_ready_) {
    return;
  }
  std::stringstream sstr;
  bool new_msg = false;
  while (Serial.available()) {
    char buf = (char) Serial.read();
    if (buf != '\n') {
      sstr << buf;
      new_msg = true;
    }
    delayMicroseconds(80);
  }
  if (new_msg) {
    std::string req_str = sstr.str();
    Serial.println(req_str.c_str());
    if (strContainsHEX(sstr.str())) {
      setCommand(SERIAL_C, strtoul(req_str.c_str(), NULL, 16));
    } else {
      if (req_str[0] == '!' && req_str[1] == 'r') {
        char c1 = 0;
        char c2 = 0;
        char c3 = 0;
        bool in_part = false;
        char current_ident = 0;
        std::stringstream partstr;

        std::string req_path;
        bool gotpath = false;

        std::string req_body;
        bool gotbody = false;

        for (int i = 2; i < req_str.size(); i++) {
          const char it_char = req_str[i];
          c1 = c2;
          c2 = c3;
          c3 = it_char;

          if (in_part) {
            if (c2 == ']' && c3 == '_') {
              in_part = false;
              Serial.println(partstr.str().c_str());
              switch(current_ident) {
                case 'p':
                  logger.printfln("Type: '%c', Value: '%s'", current_ident, sstr.str().c_str());
                  req_path = partstr.str();
                  gotpath = true;
                  break;
                case 'b':
                  logger.printfln("Type: '%c', Value: '%s'", current_ident, sstr.str().c_str());
                  req_body = partstr.str();
                  gotbody = true;
                  break;
                default:
                  logger.printfln(LOG_TYPE::ERR, "Unknown type '%c'", current_ident);
              }
              partstr = stringstream();
            } else {
              if (c2 != 0) {
                partstr << c2;
              }
            }
          } else {
            if (c1 == '_' && (int) c2 > 96 && (int) c2 < 123 && c3 == '[') {
              in_part = true;
              current_ident = c2;
              c1 = 0;
              c2 = 0;
              c3 = 0;
            }
          }
          if (gotpath && gotbody) {
            using std::placeholders::_1;
            auto *req = new Request(req_path, req_body, std::bind(&Request_Gadget::sendRequest, this, _1));
            addIncomingRequest(req);
          }
        }
      }
    }
  }
  sendQueuedItems();
}