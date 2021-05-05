#include "serial_gadget.h"
#include <sstream>

static const char *TAG = "SerialGadget";

void SerialGadget::executeRequestSending(std::shared_ptr<Request> req) {
  Serial.printf("!r_p[%s]_b[%s]_\n", req->getPath().c_str(), req->getBody().c_str());
}

SerialGadget::SerialGadget() :
    RequestGadget(RequestGadgetType::SERIAL_G) {
  logger_i(TAG, "Creating Serial Gadget");
  logger_i(TAG, "Using default Serial Connection");
  request_gadget_is_ready_ = true;
}

void SerialGadget::refresh_network() {
  receiveSerialRequest();
  sendQueuedItems();
}

void SerialGadget::receiveSerialRequest() {
  std::stringstream s_str;
  bool new_msg = false;
  while (Serial.available()) {
    char buf = (char) Serial.read();
    if (buf != '\n') {
      s_str << buf;
      new_msg = true;
    }
    delayMicroseconds(80);
  }
  if (new_msg) {
    logger_i(TAG, "Received: %s", s_str.str().c_str());

    std::string req_str = s_str.str();

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
            switch (current_ident) {
              case 'p':
                req_path = partstr.str();
                gotpath = true;
                break;
              case 'b':
                req_body = partstr.str();
                gotbody = true;
                break;
              default:
                break;
            }
            partstr = std::stringstream();
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
      }

      if (gotpath && gotbody) {
        DynamicJsonDocument doc(2056);
        deserializeJson(doc, req_body);
        if (!doc.containsKey("session_id")) {
          logger_w(TAG, "Received request without session id");
          return;
        }
        if (!doc.containsKey("sender")) {
          logger_w(TAG, "Received request without sender");
          return;
        }
        if (!doc.containsKey("receiver")) {
          logger_w(TAG, "Received request without receiver");
          return;
        }
        if (!doc.containsKey("payload")) {
          logger_w(TAG, "Received request without payload");
          return;
        }
        using std::placeholders::_1;
        auto req = std::make_shared<Request>(req_path,
                                             doc["session_id"].as<int>(),
                                             doc["sender"].as<std::string>(),
                                             doc["receiver"].as<std::string>(),
                                             doc["payload"],
                                             std::bind(&RequestGadget::sendRequest, this, _1));
        addIncomingRequest(req);
      } else {
        logger_w(TAG, "Received faulty request");
      }
    }
  }
}
