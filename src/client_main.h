#pragma once

// Tools
#include "console_logger.h"
#include "system_timer.h"
#include "boot_mode.h"
#include "static_info.h"

#include "storage/config.h"
#include "api/api_manager_delegate.h"
#include "api/api_manager.h"

#include "connectors/request_gadget.h"
#include "storage/system_storage.h"

class ClientMain : public ApiManagerDelegate {
private:

  // Name of the client to be identified in the network
  std::string client_id_;

  // Runtime id, number generated at startup to identify reboots to network partners
  int runtime_id_;

  // System config management object
  std::shared_ptr<SystemStorage> system_storage_;

  // Helper to handle all incoming and outgoing network traffic
  std::shared_ptr<ApiManager> api_manager_;

  // Network-connector to send and receive requests
  std::shared_ptr<RequestGadget> network_;

  /**
   * Loads and returns the config for initialization
   * @return The loaded config object
   */
  Config loadConfig();

  /**
   * Initializes the network gadget
   * @param mode The mode the chip should start at
   * @return Whether initializing network was successful or not
   */
  bool initNetwork(const Config& config);

  /**
   * Initializes the api manager
   * @return Whether initializing network was successful or not
   */
  bool initApi();

public:
  ClientMain();
};