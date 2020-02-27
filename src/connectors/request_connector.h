#ifndef __Request_Connector__
#define __Request_Connector__

class Request_Connector {
protected:


public:
  virtual void decodeRequest(REQUEST_TYPE type, const char *path, const char *body) {};

  virtual bool decodeRequest(REQUEST_TYPE type, const char *path, JsonObject body) {};

};

#endif // __Request_Connector__