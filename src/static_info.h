#pragma once

#include "pin_profile.h"
#include <ArduinoJson.h>
#include <sstream>

#define SW_DATA_DEFAULT "<unknown>"

/**
 * Gets the name of the git branch of the currently running software
 * @return The git branch name
 */
inline std::string getSoftwareGitBranch() {
  #ifdef SW_GIT_BRANCH
  return SW_GIT_BRANCH;
  #else
  return SW_DATA_DEFAULT;
  #endif
}

/**
 * Gets the git commit hash of the currently running software
 * @return The git commit hash
 */
inline std::string getSoftwareGitCommit() {
  #ifdef SW_GIT_COMMIT
  return SW_GIT_COMMIT;
  #else
  return SW_DATA_DEFAULT;
  #endif
}

/**
 * Gets the date the currently running software was flashed
 *
 * Format: yyyy-mm-dd
 * @return The date the current software was flashed
 */
inline std::string getSoftwareFlashDate() {
  #ifdef SW_FLASH_TIME
  return SW_FLASH_TIME;
  #else
  return SW_DATA_DEFAULT;
  #endif
}

/**
 * Generates a json document containing the complete port mapping
 * @return The port mapping as json document
 */
inline DynamicJsonDocument getPortMapping() {
  DynamicJsonDocument doc(400);
  for (int i = 0; i < MAX_PORT_INDEX; i++) {
    auto pin = getPinForPort(i);
    std::stringstream sstr;
    sstr << i;
    if (pin != 0) {
      doc[sstr.str()] = pin;
    }
  }
  return doc;
}