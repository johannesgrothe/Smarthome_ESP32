#pragma once

#include <vector>

//general
#define SERIAL_SPEED 115200
#define EEPROM_SIZE 2000

// Client
#define CLIENT_NAME_LEN_MAX 30

// gadget-Command-Mapping
#define MAPPING_MAX_COMMANDS 15
#define MAPPING_MAX_CODES 10
#define MAPPING_MAX_COMMAND_NAME_LEN 25
#define MAIN_MAX_GADGETS 20
#define GADGET_NAME_LEN_MAX 25

#define REQUEST_QUEUE_LEN 5

// CodeCommands
#define CODE_BUFFER_SIZE 15
#define CODE_TIME_GAP 150
#define CODE_TYPE_NAME_LEN 30
#define CODE_STR_LEN_MAX 300

// Homebridge
#define HOMEBRIDGE_REGISTER_STR_MAX_LEN 240
#define HOMEBRIDGE_UNREGISTER_STR_MAX_LEN 50
#define HOMEBRIDGE_UPDATE_STR_LEN_MAX 240
#define HOMEBRIDGE_SERVICE_TYPE_LEN_MAX 25

// System Storage
#define GADGET_PIN_BLOCK_LEN 5

// Paths that are used by broadcast requests
static const std::vector<std::string> broadcast_request_paths = {"smarthome/broadcast/req"};

// Paths that are used by system requests
static const std::vector<std::string> system_request_paths = {"smarthome/config/read", "smarthome/config/write", "smarthome/config/reset", "smarthome/sys", "smarthome/gadget/add", "smarthome/gadget/read"};

// Paths that are used by any other component but the core system
static const std::vector<std::string> additional_request_paths = {};