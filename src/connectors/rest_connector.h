#ifndef __REST_Connector__
#define __REST_Connector__

#include <Arduino.h>
//To  be done by the one and only Erich Honnecker!
// Gadget to communicate with REST Clients/Servers
class REST_Gadget {
protected:

public:

};


// Connector for REST Usage
class REST_Connector {
protected:
  bool decode_rest() {
    Serial.println("[WARN] decode_rest() not implemented");
    return false;
  };

  bool send_rest() {
    Serial.println("[WARN] send_rest() not implemented");
    return false;
  };

public:
  REST_Connector();
};

#endif