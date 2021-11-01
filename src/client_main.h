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
#include "scheduled_messages_manager.h"

#include "gadget_manager.h"
#include "event_manager.h"

class ClientMain : public ApiManagerDelegate {
private:

  // Mode the system is supposed to be running in
  BootMode system_mode_;

  // System config management object
  std::shared_ptr<SystemStorage> system_storage_;

  // Helper to handle all incoming and outgoing network traffic
  std::shared_ptr<ApiManager> api_manager_;

  // Manages the sending of scheduled messages like the heartbeat
  std::shared_ptr<ScheduledMessagesManager> scheduled_messages_;

  // Stores and manages all gadgets
  std::shared_ptr<GadgetManager> gadget_manager_;

  // Stores and handles all events
  std::shared_ptr<EventManager> event_manager_;

  // Network-connector to send and receive requests
  std::shared_ptr<RequestGadget> network_;

  // Infrared-gadget receiving and/or sending infrared codes
  std::shared_ptr<IR_Gadget> ir_gadget_;

  // Radio-gadget receiving and/or sending 433mhz-codes
  std::shared_ptr<Radio_Gadget> radio_gadget_;

  //region API_MANAGER_DELEGATE

  void handleGadgetUpdate(GadgetMeta gadget) override;

  void handleEvent(Event event) override;

  ClientMeta getClientData() override;

  std::vector<GadgetMeta> getGadgetData() override;

  //endregion API_MANAGER_DELEGATE

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
   * Initialized all of the gadgets stored in the config
   * @return Whether initializing all gadgets was successful or not
   */
  bool initGadgets(const GadgetConfig &config);

  /**
   * Initializes the api manager
   * @return Whether initializing network was successful or not
   */
  bool initApi(const std::string& client_id);

  /**
   * Initialized the Event Manager with the event mapping from the config
   * @param config The config to get mapping data from
   * @return Whether initializing the Event Manager was successful or not
   */
  bool initEventMapping(const EventConfig &config);

public:
  ClientMain(BootMode boot_mode, const SystemConfig& system_config, const GadgetConfig& gadget_config, const EventConfig& event_config);

  void setStorageManager(const std::shared_ptr<SystemStorage>& storage);

  void loopSystem();

  void loopGadgets();

  void loopNetwork();
};
