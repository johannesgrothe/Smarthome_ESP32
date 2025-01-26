/**
 * Main file for the Smarthome ESP32 project
 */

//region IMPORTS


#include "storage/eeprom_storage.h"
#include "storage/static_storage.h"
#include "boot_mode.h"
#include "api/api_manager.h"
#include "client_manager.h"
#include "variants/load_variant.h"
#include "gadget_manager.h"
#include "scheduled_messages_manager.h"
#include "network_loader.h"

//endregion

//region GLOBAL VARIABLES

static const char *TAG = "Initialization";

// Main class instance, handles the complete system
//std::shared_ptr<ClientMain> client_main;

// Mode the system is supposed to be running in
BootMode system_mode;

// Storage to load and save configs
std::shared_ptr<SystemStorage> storage;

// Client-Manager as API delegate
std::shared_ptr<ClientManager> client_manager;

// Helper to handle all incoming and outgoing network traffic
std::shared_ptr<ApiManager> api_manager;

// Stores and manages all gadgets
std::shared_ptr<GadgetManager> gadget_manager;

// Stores and handles all events
std::shared_ptr<EventManager> event_manager;

// Manages the sending of scheduled messages like the heartbeat
std::shared_ptr<ScheduledMessagesManager> scheduled_messages;

// Network-connector to send and receive requests
std::shared_ptr<RequestGadget> network;

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
                                            nullptr,
                                            nullptr,
                                            nullptr,
                                            nullptr,
                                            nullptr,
                                            nullptr);
  return cfg;
}

std::shared_ptr<EolConfig> loadBackupEolConfig() {
  auto cfg = std::make_shared<EolConfig>(HwVariant::unknown,
                                         "empty_serial");
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
  logger_i(TAG, "Software Info:");
  logger_i(TAG, "Flash Date: %s", getSoftwareFlashDate().c_str());
  logger_i(TAG, "Git Branch: %s", getSoftwareGitBranch().c_str());
  logger_i(TAG, "Git Commit: %s", getSoftwareGitCommit().c_str());
  logger_i(TAG, "API Version: %d.%d.%d",
           api_definitions::version::major,
           api_definitions::version::minor,
           api_definitions::version::bugfix);

#ifdef STATIC_CONFIG_ACTIVE
  storage = std::make_shared<StaticStorage>();
#else
  storage = std::make_shared<EepromStorage>();
#endif

  auto boot_mode = getBootMode();

  auto eol_config = storage->loadEolConfig();
  if (eol_config == nullptr) {
    logger_e(TAG, "Could not load system config, falling back to setup mode");
    boot_mode = BootMode::Serial_Only;
    eol_config = loadBackupEolConfig();
  }

  auto system_config = storage->loadSystemConfig();
  if (system_config == nullptr) {
    logger_e(TAG, "Could not load system config, falling back to setup mode");
    boot_mode = BootMode::Serial_Only;
    system_config = loadBackupSystemConfig();
  }

  logger_i(TAG, "Initializing gadget manager");
  gadget_manager = std::make_shared<GadgetManager>();

  logger_i(TAG, "Initializing Gadgets:");
  loadGadgets(gadget_manager);

  logger_i(TAG, "Initializing event manager");
  event_manager = std::make_shared<EventManager>(std::vector<event_map>());

  client_manager = std::make_shared<ClientManager>(boot_mode,
                                                   eol_config->variant,
                                                   eol_config->serial,
                                                   SW_VARIANT,
                                                   storage,
                                                   gadget_manager,
                                                   event_manager);

  logger_i(TAG, "Initializing Network");
  if (boot_mode == BootMode::Serial_Only) {
    network = NetworkLoader::loadMqtt(*system_config);
  } else {
    network = NetworkLoader::loadSerial();
  }

  if (network == nullptr) {
    HardwareController::rebootChip("Network initialization failed.", 15);
  }

  api_manager = std::make_shared<ApiManager>(network);

  client_main = std::make_shared<ClientMain>(boot_mode, *system_config);
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
