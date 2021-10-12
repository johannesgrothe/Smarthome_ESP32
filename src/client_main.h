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
#include "connectors/ir_gadget.h"
#include "connectors/radio_gadget.h"

class ClientMain : public ApiManagerDelegate {
private:

  // Name of the client to be identified in the network
  std::string client_id_;

  // Runtime id, number generated at startup to identify reboots to network partners
  uint16_t runtime_id_;

  // Mode the system is supposed to be running in
  BootMode system_mode_;

  // System config management object
  std::shared_ptr<SystemStorage> system_storage_;

  // Helper to handle all incoming and outgoing network traffic
  std::shared_ptr<ApiManager> api_manager_;

  // Network-connector to send and receive requests
  std::shared_ptr<RequestGadget> network_;

  // Infrared-gadget receiving and/or sending infrared codes
  std::shared_ptr<IR_Gadget> ir_gadget;

  // Radio-gadget receiving and/or sending 433mhz-codes
  std::shared_ptr<Radio_Gadget> radio_gadget;

  //region API_MANAGER_DELEGATE

  void handleGadgetUpdate(GadgetMeta gadget) override;

  void handleCode(CodeCommand code) override;

  void handleEvent(Event event) override;

  std::string getClientId() override;

  ClientMeta getClientData() override;

  std::vector<GadgetMeta> getGadgetData() override;

  //endregion API_MANAGER_DELEGATE

  /**
   * Loads and returns the config for initialization
   * @return The loaded config object
   */
  SystemConfig loadSystemConfig();

  /**
   * Initializes the network gadget
   * @param mode The mode the chip should start at
   * @return Whether initializing network was successful or not
   */
  bool initNetwork(const SystemConfig &config, NetworkMode mode);

  /**
   * Initializes all Connectors (IR/Radio)
   * @return Whether initializing connectors was successful or not
   */
  bool initConnectors(const SystemConfig &config);

  /**
   * Initialized all of the gadgets stored in the EEPROM
   * @return Whether initializing all gadgets was successful or not
   */
  bool initGadgets(const GadgetConfig &config);

  /**
   * Initializes the api manager
   * @return Whether initializing network was successful or not
   */
  bool initApi();

public:
  ClientMain();
};