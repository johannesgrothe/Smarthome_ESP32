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