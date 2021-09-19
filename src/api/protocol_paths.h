#pragma once

#include <vector>

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
#define PATH_CONFIG_READ "smarthome/config/read"
#define PATH_GADGET_WRITE "smarthome/gadget/add"

#define PATH_HEARTBEAT "smarthome/heartbeat"
#define PATH_SYNC_REQ "smarthome/sync"

// Testing
#define PATH_ECHO_TEST "test"

// Inbound
#define PATH_CONFIG_RESET "config/reset"
#define PATH_CONFIG_WRITE "config/write"

#define PATH_SYNC_GADGET "sync/gadget"
#define PATH_SYNC_CODE "sync/code"
#define PATH_SYNC_EVENT "sync/event"

// Outbound
#define PATH_PUBLISH_CLIENT "sync/client"
#define PATH_PUBLISH_CODE "publish/code"
#define PATH_PUBLISH_EVENT "publish/event"

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
static const std::vector<std::string> additional_request_paths = {PATH_SYNC_REQ,
                                                                  PATH_ECHO_TEST};

static const std::vector<std::string> api_paths_broadcast_enabled = {PATH_SYNC_REQ,
                                                                     PATH_ECHO_TEST,
                                                                     PATH_SYNC_GADGET,
                                                                     PATH_SYNC_CODE,
                                                                     PATH_SYNC_EVENT};

static const std::vector<std::string> api_paths_incoming = {PATH_SYNC_REQ,
                                                            PATH_ECHO_TEST,
                                                            PATH_SYNC_GADGET,
                                                            PATH_SYNC_CODE,
                                                            PATH_SYNC_EVENT,
                                                            PATH_CONFIG_RESET,
                                                            PATH_CONFIG_WRITE};
