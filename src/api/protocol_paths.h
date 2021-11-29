#pragma once

#include <vector>

#include "../system/api_params.h"

//#define PATH_HEARTBEAT "heartbeat"
//#define PATH_SYNC_REQ "sync"

// Testing
//#define PATH_ECHO_TEST "test"

//// Inbound
//#define PATH_CONFIG_RESET "config/reset"
//#define PATH_CONFIG_GADGETS_WRITE "config/gadgets/write"
//#define PATH_CONFIG_EVENTS_WRITE "config/events/write"
//#define PATH_CONFIG_SYSTEM_WRITE "config/system/write"
//#define PATH_SYNC_GADGET "sync/gadget"
//#define PATH_SYNC_CODE "sync/code"
//#define PATH_SYNC_EVENT "sync/event"
//
//// Outbound
//#define PATH_PUBLISH_CLIENT "sync/client"
//#define PATH_PUBLISH_EVENT "publish/event"

static const std::vector<std::string> api_paths_broadcast_enabled = {PATH_SYNC_REQUEST,
                                                                     PATH_TEST_ECHO,
                                                                     PATH_SYNC_GADGET,
                                                                     PATH_SYNC_EVENT};

static const std::vector<std::string> api_paths_incoming = {PATH_SYNC_REQUEST,
                                                            PATH_TEST_ECHO,
                                                            PATH_SYNC_GADGET,
                                                            PATH_SYNC_EVENT,
                                                            PATH_CLIENT_CONFIG_DELETE,
                                                            PATH_CLIENT_GADGET_CONFIG_WRITE,
                                                            PATH_CLIENT_EVENT_CONFIG_WRITE,
                                                            PATH_CLIENT_SYSTEM_CONFIG_WRITE};
