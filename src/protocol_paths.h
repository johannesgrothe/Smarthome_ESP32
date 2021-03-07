#pragma once

// Names and other constants
#define PROTOCOL_BRIDGE_NAME "<bridge>"

// Sending
#define PATH_CODE_UPDATE_TO_BRIDGE "smarthome/to/code"
#define PATH_CHARACTERISTIC_UPDATE_TO_BRIDGE "smarthome/remotes/gadget/update"
#define PATH_EVENT_UPDATE_TO_BRIDGE "smarthome/remotes/event/send"

// Receiving
#define PATH_CODE_UPDATE_FROM_BRIDGE "smarthome/from/code"
#define PATH_CHARACTERISTIC_UPDATE_FROM_BRIDGE "smarthome/from/set"
#define PATH_EVENT_UPDATE_FROM_BRIDGE "smarthome/remotes/event/receive"

#define PATH_BROADCAST "smarthome/broadcast/req"
#define PATH_SYSTEM_CONTROL "smarthome/sys"
#define PATH_CONFIG_RESET "smarthome/config/reset"
#define PATH_CONFIG_WRITE "smarthome/config/write"
#define PATH_CONFIG_READ "smarthome/config/read"
#define PATH_GADGET_WRITE "smarthome/gadget/add"

#define PATH_HEARTBEAT "smarthome/heartbeat"
#define PATH_SYNC "smarthome/sync"
#define PATH_TEST "smarthome/test"

// Paths that are used by broadcast requests
static const std::vector<std::string> broadcast_request_paths = {PATH_BROADCAST};

// Paths that are used by system requests
static const std::vector<std::string> system_request_paths = {PATH_CONFIG_READ,
                                                              PATH_CONFIG_WRITE,
                                                              PATH_CONFIG_RESET,
                                                              PATH_SYSTEM_CONTROL,
                                                              PATH_GADGET_WRITE,
                                                              PATH_CONFIG_READ};

// Paths that are used by any other component but the core system
static const std::vector<std::string> additional_request_paths = {PATH_SYNC,
                                                                  PATH_TEST};