#ifndef __REST_Connector__
#define __REST_Connector__

#include <Arduino.h>
#include "../console_logger.h"
#include "../system_settings.h"
#include "../../../../../.platformio/packages/framework-arduinoespressif32/libraries/WiFi/src/WiFiClient.h"


enum HTTP_TYPE {
  HTTP_GET, HTTP_POST, HTTP_UPDATE, HTTP_DELETE
};

//To  be done by the one and only Erich Honnecker!
// Gadget to communicate with REST Clients/Servers
class REST_Gadget {
protected:
  bool is_initialized;
  WiFiClient *momonga;
  bool has_request;
  HTTP_TYPE type;
  char body[REST_MAX_BODY_LEN]{};
  char path[REST_MAX_PATH_LEN]{};

public:
  REST_Gadget() :
      is_initialized(false),
      momonga(nullptr),
      has_request(false),
      type(HTTP_GET) {


  };

  explicit REST_Gadget(WiFiClient *blubb) :
      is_initialized(true),
      momonga(blubb),
      has_request(false),
      type(HTTP_GET) {

  };

  bool hasRequest() {
    bool buffer = has_request;
    has_request = false;
    return buffer;
  }

  HTTP_TYPE getRequestType() {
    return type;
  }

  const char *getRequestBody() {
    return &body[0];
  }

  const char *getRequestPath() {
    return &path[0];
  }

  void refresh() {

  }


};


/* functions:
 *      void refresh() => check if wifi available, refresh connection, get last command (POST),
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * Constructor needs ptr to WIFIClient Object
 *
 *
 *
 *
 *
 *  methods you need to acces from the outside:
 *  bool hasNewRequest() -> returns true if there has been a request since asking
 *  HTTP_TYPE getLastRequestType() -> returns GET, POST, UPDATE, DELETE
 *  const char * getLastRequestBody() -> returns the body of the last request
 *  const char * getLastRequestPath() -> returns the Path of the last request
 *
 *
 *
 *
 *
 *
 * */


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