#ifndef __Remote_Manager__
#define __Remote_Manager__

#include "homebridge_remote.h"

class Remote_Manager {
private:

  Remote *remotes[REMOTE_MANAGER_MAX_REMOTES]{};

  byte remote_count;

public:
  Remote_Manager() {};

  void updateRemotes(const char *gadget_name, const char *service, const char *characteristic, int value) {
    for (byte k = 0; k < remote_count; k++) {
      remotes[k]->updateCharacteristic(gadget_name, service, characteristic, value);
    }
  }

  void updateRemotes(const char *gadget_name, const char *service, const char *characteristic, bool value) {
    for (byte k = 0; k < remote_count; k++) {
      remotes[k]->updateCharacteristic(gadget_name, service, characteristic, value);
    }
  }

  void forwardRequest(const char *path, REQUEST_TYPE type, const char *body) {
    for (byte k = 0; k < remote_count; k++) {
      remotes[k]->handleRequest(path, type, body);
    }
  }

  void forwardRequest(const char *path, REQUEST_TYPE type, JsonObject body) {
    for (byte k = 0; k < remote_count; k++) {
      remotes[k]->handleRequest(path, type, body);
    }
  }

  void addRemote(Remote *new_remote) {
    if (remote_count < (REMOTE_MANAGER_MAX_REMOTES - 1)) {
      remotes[remote_count] = new_remote;
      remote_count++;
    }
  }

};

#endif //__Remote_Manager__