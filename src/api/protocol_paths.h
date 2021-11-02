#pragma once

#include <vector>

#define PATH_HEARTBEAT "smarthome/heartbeat"
#define PATH_SYNC_REQ "smarthome/sync"

// Testing
#define PATH_ECHO_TEST "test"

// Inbound
#define PATH_CONFIG_RESET "config/reset"
#define PATH_CONFIG_GADGETS_WRITE "config/gadgets/write"
#define PATH_CONFIG_EVENTS_WRITE "config/events/write"
#define PATH_CONFIG_SYSTEM_WRITE "config/system/write"
#define PATH_SYNC_GADGET "sync/gadget"
#define PATH_SYNC_CODE "sync/code"
#define PATH_SYNC_EVENT "sync/event"

// Outbound
#define PATH_PUBLISH_CLIENT "sync/client"
#define PATH_PUBLISH_EVENT "publish/event"

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
                                                            PATH_CONFIG_GADGETS_WRITE,
                                                            PATH_CONFIG_EVENTS_WRITE,
                                                            PATH_CONFIG_SYSTEM_WRITE};
