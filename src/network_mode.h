#pragma once

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
