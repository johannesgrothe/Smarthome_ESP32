#include "api_manager.h"

#include <utility>
#include <memory>

#include "protocol_paths.h"
#include "api_encoder.h"
#include "api_decoder.h"

ApiManager::ApiManager(ApiManagerDelegate *delegate, std::shared_ptr<RequestGadget> network, uint16_t runtime_id,
                       std::string client_identifier) :
    client_id_(std::move(client_identifier)),
    runtime_id_(runtime_id),
    delegate_(delegate),
    network_(std::move(network)) {};

void ApiManager::handleRequest(const std::shared_ptr<Request> &req) {
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
    req->updateReceiver(client_id_);
  } else if (client_id_ != req->getReceiver()) {
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
    auto sender = req->getSender();
    publishSync(&sender);
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

uint16_t ApiManager::genRequestID() {
  return random_int(10000);
}

// region PATH CHECKERS

bool ApiManager::pathIsLegal(const std::string &path) {
  return (std::any_of(api_paths_incoming.begin(),
                      api_paths_incoming.end(),
                      [&path](const std::string &list_path) { return list_path == path; }));
}

bool ApiManager::pathIsBroadcast(const std::string &path) {
  return (std::any_of(api_paths_broadcast_enabled.begin(),
                      api_paths_broadcast_enabled.end(),
                      [&path](const std::string &list_path) { return list_path == path; }));
}

// endregion PATH CHECKERS

// region HANDLE INCOMING REQUESTS

void ApiManager::handleEcho(const std::shared_ptr<Request> &req) {
  DynamicJsonDocument request = req->getPayload();
  req->respond(req->getPath(), request);
};

void ApiManager::handleGadgetUpdate(const std::shared_ptr<Request> &req) {
  if (req->getPayload()["gadget"].is<JsonObject>()) {
    auto gadget_meta = ApiDecoder::decodeGadget(req->getPayload()["gadget"]);
    delegate_->handleGadgetUpdate(gadget_meta);
  }
}

void ApiManager::handleCodeUpdate(const std::shared_ptr<Request> &req) {
  // TODO: implement forwarding incoming code
}

void ApiManager::handleEventUpdate(const std::shared_ptr<Request> &req) {
  // TODO: implement forwarding incoming event
}

// endregion HANDLE INCOMING REQUESTS

// region PUBLISH CHANGES

void ApiManager::publishSync(std::string *receiver = nullptr) {
  auto client_data = delegate_->getClientData();
  auto gadget_data = delegate_->getGadgetData();
  auto payload = ApiEncoder::encodeSync(client_data, gadget_data, runtime_id_);
  if (receiver == nullptr) {
    auto out_req = std::make_shared<Request>(PATH_PUBLISH_CLIENT,
                                             genRequestID(),
                                             client_id_,
                                             payload);
    network_->sendRequest(out_req);
  } else {
    auto out_req = std::make_shared<Request>(PATH_PUBLISH_CLIENT,
                                             genRequestID(),
                                             client_id_,
                                             *receiver,
                                             payload);
    network_->sendRequest(out_req);
  }
}

void ApiManager::publishGadgetUpdate(const GadgetMeta &gadget_data) {
  auto payload = ApiEncoder::encodeGadgetUpdate(gadget_data);
  auto out_req = std::make_shared<Request>(PATH_SYNC_GADGET,
                                           genRequestID(),
                                           client_id_,
                                           payload);
  network_->sendRequest(out_req);
}

void ApiManager::publishEvent(const std::shared_ptr<Event> &event) {
  // TODO: implement event publishing
}

void ApiManager::publishHeartbeat() {
  auto payload = ApiEncoder::encodeHeartbeat(runtime_id_);

  auto heartbeat_request = std::make_shared<Request>(PATH_HEARTBEAT,
                                                     genRequestID(),
                                                     client_id_,
                                                     payload);
  network_->sendRequest(heartbeat_request);
}

// endregion PUBLISH CHANGES
