#pragma once

#include "connectors/ir_gadget.h"
#include "connectors/serial_gadget.h"
#include "connectors/radio_gadget.h"

/**
 * Every possible network mode
 */
enum class NetworkMode {
  None,
  Serial,
  MQTT
};

/**
 * Count of possibilities in NetworkMode
 */
#define NetworkModeCount 3
