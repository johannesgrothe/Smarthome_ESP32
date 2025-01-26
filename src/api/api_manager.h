#pragma once

#include <gadget_manager.h>
#include <memory>
#include "api_manager_delegate.h"
#include "../connectors/request_gadget.h"
#include "../connectors/request.h"
#include "../connectors/event.h"

class ApiManager {
    // Name of the client to be identified in the network
    const std::string client_id_;

    // Runtime id, number generated at startup to identify reboots to network partners
    const uint16_t runtime_id_;

    // Network to perform API operations on
    std::shared_ptr<RequestGadget> network_;

    // Delegate to get information from and write information to
    std::shared_ptr<ApiManagerDelegate> delegate_;

    static uint16_t genRequestID();

    static bool pathIsLegal(const std::string &path);

    static bool pathIsBroadcast(const std::string &path);

    static void handleEcho(const std::shared_ptr<Request> &req);

    void handleGadgetUpdate(const std::shared_ptr<Request> &req) const;

    void handleEventUpdate(const std::shared_ptr<Request> &req);

    void handleConfigReset(const std::shared_ptr<Request> &req);

    void handleSystemConfigWrite(const std::shared_ptr<Request> &req) const;

    void handleEventConfigWrite(const std::shared_ptr<Request> &req);

    void handleGadgetConfigWrite(const std::shared_ptr<Request> &req);

public:
    ApiManager(std::shared_ptr<ApiManagerDelegate> delegate,
               std::shared_ptr<RequestGadget> network,
               uint16_t runtime_id,
               std::string client_identifier);

    void handleRequest(const std::shared_ptr<Request> &req);

    void publishSync(std::string *receiver) const;

    void publishGadgetUpdate(const GadgetUpdateMeta &gadget_data) const;

    void publishEvent(const std::shared_ptr<Event> &event) const;

    void publishHeartbeat() const;
};
