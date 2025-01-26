#pragma once

// Tools
#include "api/dtos/gadget_update_dto.h"
#include "console_logger.h"
#include "boot_mode.h"
#include "api/api_manager_delegate.h"
#include "api/api_manager.h"
#include "storage/system_storage.h"
#include "gadget_manager.h"
#include "event_manager.h"

class ClientManager final : public ApiManagerDelegate {
    // Mode the system is supposed to be running in
    BootMode system_mode_;

    // Hardware Variant the system is running on
    SwVariant sw_variant_;

    // Software variant the system is running with
    HwVariant hw_variant_;

    // Serial number of hardware
    std::string hw_serial_;

    // System config management object
    std::shared_ptr<SystemStorage> system_storage_;

    // Stores and manages all gadgets
    std::shared_ptr<GadgetManager> gadget_manager_;

    // Stores and handles all events
    std::shared_ptr<EventManager> event_manager_;

    //region API_MANAGER_DELEGATE

    void handleGadgetUpdate(GadgetUpdateDTO gadget) override;

    void handleEvent(Event event) override;

    bool handleSystemConfigWrite(SystemConfig cfg) override;

    ClientDTO getClientData() override;

    std::vector<GadgetDTO> getGadgetData() override;

    //endregion API_MANAGER_DELEGATE

public:
    ClientManager(BootMode boot_mode,
                  HwVariant hw_variant,
                  std::string hw_serial,
                  SwVariant sw_variant,
                  std::shared_ptr<SystemStorage> system_storage,
                  std::shared_ptr<GadgetManager> gadget_manager,
                  std::shared_ptr<EventManager> event_manager);
};
