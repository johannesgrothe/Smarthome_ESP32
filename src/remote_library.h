#pragma once

// include all possible remotes here
#include "remotes/smarthome_gadget_remote.h"
#include "remotes/smarthome_code_remote.h"

/**
 * Every possible gadget remote mode
 */
enum class GadgetRemoteMode {
  None,
  Smarthome
};

/**
 * Count of possibities in GadgetRemoteMode
 */
#define GadgetRemoteModeCount 2

/**
 * Every possible code remote mode
 */
enum class CodeRemoteMode {
None,
Smarthome
};

/**
 * Count of possibities in CodeRemoteMode
 */
#define CodeRemoteModeCount 2

/**
 * Every possible event remote mode
 */
enum class EventRemoteMode {
  None,
  Smarthome
};

/**
 * Count of possibities in EventRemoteMode
 */
#define EventRemoteModeCount 2
