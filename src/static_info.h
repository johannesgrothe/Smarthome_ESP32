#pragma once
#include "pin_profile.h"
#include "ArduinoJson.h"

#define SW_DATA_DEFAULT "<unknown>"

/**
 * Gets the name of the git branch of the currently running software
 * @return the git branch name
 */
std::string getSoftwareGitBranch() {
  #ifdef SW_GIT_BRANCH
  return SW_GIT_BRANCH;
  #else
  return SW_DATA_DEFAULT;
  #endif
}

/**
 * Gets the git commit hash of the currently running software
 * @return
 */
std::string getSoftwareGitCommit() {
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
std::string getSoftwareFlashDate() {
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
DynamicJsonDocument getPortMapping() {
  DynamicJsonDocument doc(300);
  for (int i = 0; i < MAX_PORT_INDEX; i++) {
    auto pin = getPinForPort(i);
    if (pin != 0) {
      doc[i] = pin;
    }
  }
  return doc;
}