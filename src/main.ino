/**
 * Main file for the Smarthome ESP32 project
 */

//region IMPORTS

#include "client_main.h"
#include "storage/eeprom_storage.h"
#include "boot_mode.h"

//endregion

//region GLOBAL VARIABLES

static const char *TAG = "Initialization";

// Main class instance, handles the complete system
std::shared_ptr<ClientMain> client_main;

// Storage to load and save configs
std::shared_ptr<SystemStorage> storage;

// Main task, handling the system in general
TaskHandle_t main_task;

// Network task, receiving and sending requests via the network gadget
TaskHandle_t network_task;

// Gadget task, looping the gadgets refresh method allowing them to update their hardware
TaskHandle_t gadget_task;

// Heartbeat task, sending a heartbeat request every 5 seconds
TaskHandle_t heartbeat_task;

//endregion

//region BACKUP CONFIGS

std::shared_ptr<SystemConfig> loadBackupSystemConfig() {
  auto cfg = std::make_shared<SystemConfig>("empty_client",
                                            NetworkMode::Serial,
                                            0,
                                            0,
                                            0,
                                            0,
                                            nullptr,
                                            nullptr,
                                            nullptr,
                                            nullptr,
                                            nullptr,
                                            nullptr);
  return cfg;
}

std::shared_ptr<EventConfig> loadBackupEventConfig() {
  std::vector<event_map> event_mapping;
  auto cfg = std::make_shared<EventConfig>(event_mapping);
  return cfg;
}

std::shared_ptr<GadgetConfig> loadBackupGadgetConfig() {
  std::vector<gadget_tuple> gadget_data;
  auto cfg = std::make_shared<GadgetConfig>(gadget_data);
  return cfg;
}

// endregion BACKUP CONFIGS

//region TASKS

/**
 * Function for the main task refresing the main content
 * @param args Unused
 */
[[noreturn]] static void mainTask(void *args) {
  while (true) {
    client_main->loopSystem();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

/**
 * Function for the network tasks receiving and sending requests
 * @param args Unused
 */
[[noreturn]] static void networkTask(void *args) {
  while (true) {
    client_main->loopNetwork();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

/**
 * Function for the gadgets refreshing their hardware
 * @param args Unused
 */
[[noreturn]] static void gadgetTask(void *args) {
  while (true) {
    client_main->loopGadgets();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

/**
 * Function for the network tasks receiving and sending requests
 * @param args Unused
 */
[[noreturn]] static void heartbeatTask(void *args) {
  while (true) {
//    sendHeartbeat();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

/**
 * Creates and starts the tasks used by the system
 */
static void createTasks() {
  logger_i(TAG, "Creating tasks...");
//  xTaskCreatePinnedToCore(
//      mainTask,              // Task function.
//      "Smarthome_Main",      // String with name of task.
//      10000,                 // Stack size in words.
//      NULL,                  // Parameter passed as input of the task
//      1,                     // Priority of the task.
//      &main_task,            // Task handle.
//      0);                    // Core to run on

  xTaskCreatePinnedToCore(
      networkTask,
      "Smarthome_Network",
      10000,
      NULL,
      1,
      &network_task,
      1);

  xTaskCreatePinnedToCore(
      gadgetTask,
      "Smarthome_Gadgets",
      10000,
      NULL,
      1,
      &gadget_task,
      1);

//  xTaskCreatePinnedToCore(
//      heartbeatTask,
//      "Smarthome_Heartbeat",
//      10000,
//      NULL,
//      1,
//      &heartbeat_task,
//      1);
  logger_i(TAG, "Tasks up and running.");
}

//endregion TASKS

//region MAIN FUNCTIONS

/**
 * Setup-method that is automatically called once on launch
 */
void setup() {
  Serial.begin(SERIAL_SPEED);

  logger_i(TAG, "Launching...");

  auto boot_mode = getBootMode();

  storage = std::make_shared<EepromStorage>();

  if (storage->getDoubleRebootFlag()) {
    boot_mode = BootMode::Serial_Only;
    logger_i(TAG, "Double reboot detected, starting in SerialOnly mode...");
  } else {
    storage->setDoubleRebootFlag(true);
    HardwareController::sleepMilliseconds(1500);
  }
  storage->setDoubleRebootFlag(false);

  auto system_config = storage->loadSystemConfig();
  auto gadget_config = storage->loadGadgetConfig();
  auto event_config = storage->loadEventConfig();

  if (system_config == nullptr) {
    logger_e(TAG, "Could not load system config, falling back to all backup configs");
    system_config = loadBackupSystemConfig();
    gadget_config = loadBackupGadgetConfig();
    event_config = loadBackupEventConfig();
  } else {
    if (gadget_config == nullptr) {
      logger_e(TAG, "Could not load gadget config, falling back to backup config");
      gadget_config = loadBackupGadgetConfig();
    }
    if (event_config == nullptr) {
      logger_e(TAG, "Could not load event mapping config, falling back to backup config");
      event_config = loadBackupEventConfig();
    }
  }

  client_main = std::make_shared<ClientMain>(boot_mode,
                                             *system_config,
                                             *gadget_config,
                                             *event_config);
  client_main->setStorageManager(storage);
  logger_i(TAG, "Main launched successfully");
  createTasks();
}

/**
 * Loop-Method that is called forever while chip is running.
 * Used for the heartbeat sending.
 */
void loop() {
  client_main->loopSystem();
}

//endregion
