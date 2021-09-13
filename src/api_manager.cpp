#include "api_manager.h"

#include "protocol_paths.h"

ApiManager::ApiManager(ApiManagerDelegate *delegate) :
    delegate_(delegate) {

};

void ApiManager::handleRequest(std::shared_ptr<Request> req) {
  std::string req_path = req->getPath();

  if (!pathIsLegal(req_path)) {
    // Quit if path is unknown to api
    return;
  }

  if (!req->hasReceiver()) {
    // Update receiver to 'self' if there is none (request is a broadcast)
    req->updateReceiver(delegate_->getClientId());
  } else if (delegate_->getClientId() != req->getReceiver()) {
    // Return if the client is not the receiver of the message
    return;
  }

  // For testing purposes
  if (req->getPath() == PATH_ECHO_TEST) {
    handleEcho(req);
    return;
  }

  // Sync request from bridge
  if (req->getPath() == PATH_SYNC_REQ) {
    publishSync();
    return;
  }

  // Sync request from bridge
  if (req->getPath() == PATH_SYNC_GADGET) {
    handleClientSync();
    return;
  }

  logger_e("ApiManager", "Received request to unhandled path");
}

bool ApiManager::pathIsLegal(std::string path) {
  for (const auto &list_path: api_paths_incoming) {
    if (list_path == path) {
      return true;
    }
  }
  return false;
}

// region HANDLE INCOMING REQUESTS

void ApiManager::handleEcho(std::shared_ptr<Request> req) {
  DynamicJsonDocument request = req->getPayload();
  req->respond(req->getPath(), request);
};

void ApiManager::handleClientSync(std::shared_ptr<Request> req) {
  // TODO: implement forwarding client sync info
}

// endregion HANDLE INCOMING REQUESTS

// region PUBLISH CHANGES

void ApiManager::publishSync() {
  // TODO: implement sync sending
}

// endregion PUBLISH CHANGES
