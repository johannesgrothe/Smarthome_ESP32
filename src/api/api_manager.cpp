#include "api_manager.h"

#include <utility>

#include "protocol_paths.h"

ApiManager::ApiManager(ApiManagerDelegate *delegate, std::shared_ptr<RequestGadget> network) :
    delegate_(delegate),
    network_(std::move(network)) {};

void ApiManager::handleRequest(std::shared_ptr<Request> req) {
  std::string req_path = req->getPath();

  if (!pathIsLegal(req_path)) {
    // Quit if path is unknown to api
    return;
  }

  if (!req->hasReceiver()) {
    if (!pathIsBroadcast(req_path)) {
      // Path is no legal broadcast
      return;
    }
    // Update receiver to 'self' if there is none (request is a broadcast)
    req->updateReceiver(delegate_->getClientId());
  } else if (delegate_->getClientId() != req->getReceiver()) {
    // Return if the client is not the receiver of the message
    return;
  }

  // Switch only works for integer types, therefore all the IFs
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

  // Gadget update from bridge
  if (req->getPath() == PATH_SYNC_GADGET) {
    handleGadgetUpdate(req);
    return;
  }

  // Code from bridge
  if (req->getPath() == PATH_SYNC_CODE) {
    handleCodeUpdate(req);
    return;
  }

  // Event from Bridge
  if (req->getPath() == PATH_SYNC_EVENT) {
    handleEventUpdate(req);
    return;
  }

  logger_e("ApiManager", "Received request to unhandled path");
}

// region PATH CHECKERS

bool ApiManager::pathIsLegal(const std::string &path) {
  return (std::any_of(api_paths_incoming.begin(),
                      api_paths_incoming.end(),
                      [&path](const std::string& list_path) { return list_path == path; }));
}

bool ApiManager::pathIsBroadcast(const std::string &path) {
  return (std::any_of(api_paths_broadcast_enabled.begin(),
                      api_paths_broadcast_enabled.end(),
                      [&path](const std::string& list_path) { return list_path == path; }));
}

// endregion PATH CHECKERS

// region HANDLE INCOMING REQUESTS

void ApiManager::handleEcho(const std::shared_ptr<Request>& req) {
  DynamicJsonDocument request = req->getPayload();
  req->respond(req->getPath(), request);
};

void ApiManager::handleGadgetUpdate(std::shared_ptr<Request> req) {
  // TODO: implement forwarding client sync info
}

void ApiManager::handleCodeUpdate(std::shared_ptr<Request> req) {
  // TODO: implement forwarding incoming code
}

void ApiManager::handleEventUpdate(std::shared_ptr<Request> req) {
  // TODO: implement forwarding incoming event
}

// endregion HANDLE INCOMING REQUESTS

// region PUBLISH CHANGES

void ApiManager::publishSync() {
  auto client_data = delegate_->getClientData();
  auto gadget_data = delegate_->getGadgetData();
  // TODO: implement sync sending
}

void ApiManager::publishGadgetUpdate(GadgetMeta gadget_data) {
  // TODO: implement code publishing
}

void ApiManager::publishCode(std::shared_ptr<CodeCommand> code) {
  // TODO: implement code publishing
}

void ApiManager::publishEvent(std::shared_ptr<Event> event) {
  // TODO: implement event publishing
}

// endregion PUBLISH CHANGES
