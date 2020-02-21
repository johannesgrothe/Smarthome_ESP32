#ifndef __Request_Connector__
#define __Request_Connector__

// External Imports
#include <ArduinoJson.h>
#include <cstring>

// Other Imports
#include "../system_settings.h"
#include "../console_logger.h"

enum REQUEST_TYPE {
  REQ_HTTP_GET, REQ_HTTP_POST, REQ_HTTP_UPDATE, REQ_HTTP_DELETE, REQ_MQTT, REQ_SERIAL
};

class Request_Gadget {
protected:
  bool is_initialized;
  bool has_request;
  REQUEST_TYPE type;
  char body[REQUEST_BODY_LEN_MAX]{};
  char path[REQUEST_PATH_LEN_MAX]{};

  void setBody(const char *new_body) {
    strncpy(body, new_body, REQUEST_BODY_LEN_MAX);
  };

  void setPath(const char *new_path) {
    strncpy(path, new_path, REQUEST_PATH_LEN_MAX);
  };

  void setRequest(char *new_path, char *new_body, REQUEST_TYPE new_type) {
    setBody(new_body);
    setPath(new_path);
    type = new_type;
  };

public:
  Request_Gadget() :
    is_initialized(false) {
  }

  Request_Gadget(JsonObject data) :
    is_initialized(false),
    has_request(false),
    type(REQ_HTTP_GET) {
  }

  bool isInitialized() {
    return is_initialized;
  }

  bool hasRequest() {
    bool buffer = has_request;
    has_request = false;
    return buffer;
  }

  REQUEST_TYPE getRequestType() {
    return type;
  }

  const char *getRequestBody() {
    return &body[0];
  }

  const char *getRequestPath() {
    return &path[0];
  }

  virtual void refresh() {
  }
};

#endif //__Request_Connector__