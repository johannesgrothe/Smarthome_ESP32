#include "client_main.h"

#include "storage/static_storage.h"
#include "storage/eeprom_storage.h"

#include "hardware_controller.h"
#include "network_loader.h"
#include "random.h"

ClientMain::ClientMain() :
    ApiManagerDelegate(),
    client_id_(""),
    runtime_id_(),
    system_storage_(nullptr),
    api_manager_(nullptr),
    network_(nullptr) {
  logger_i("System", "Launching...");

  runtime_id_ = random_int(10000);
  logger_i("System", "Runtime ID: %d", runtime_id_);

  logger_i("System", "Software Info:");
  logger_i("System", "Flash Date: %s", getSoftwareFlashDate().c_str());
  logger_i("System", "Git Branch: %s", getSoftwareGitBranch().c_str());
  logger_i("System", "Git Commit: %s", getSoftwareGitCommit().c_str());

  logger_i("System", "Initializing Storage:");

  if (StaticStorage::staticConfigStringAvailable()) {
    logger_i("System", "Using static, pre-compiled config");
    system_storage_ = std::make_shared<StaticStorage>();
  } else {
    logger_i("System", "Using dynamic, EEPROM config");
    system_storage_ = std::make_shared<EepromStorage>();
  }

  if (!system_storage_->isInitialized()) {
    HardwareController::rebootChip("System storage initialization error", 15);
  }

  auto system_config = loadConfig();

  client_id_ = system_config.getID();
  logger_i("System", "Client ID: '%s'", client_id_.c_str());

  initNetwork(system_config);
  initApi();
}

Config ClientMain::loadConfig() {
  auto config = system_storage_->loadConfig();
  if (config == nullptr) {
    logger_e("System", "Failed to load system configuration data");
    HardwareController::rebootChip("Config loading error", 15);
  }
  logger_i("System", "Config loaded successfully");
  return *config;
}

bool ClientMain::initNetwork(const Config& config) {
  auto network = NetworkLoader::loadNetwork(config);
  if (network == nullptr) {
    return false;
  }
  network_ = network;
  return true;
}

bool ClientMain::initApi() {
  api_manager_ = std::make_shared<ApiManager>(this, network_);
  return true;
}
