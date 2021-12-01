#pragma once

#ifdef UNIT_TEST
#include "configs/config_unit_test.h"
#else
//#include "configs/config_test1.h"
#include "configs/config_test2.h"
#endif

// The maximum port index allowed
#define MAX_PORT_INDEX 9

/**
 * Returns the configured pin for the given port.
 * Returns 0 if the port is not configured.
 * @param port The port to get the pin for
 * @return The number of the physical pin mapped to the port
 */
static uint8_t getPinForPort(uint8_t port) {
  switch (port) {
    case 0:
      return 0; // 0 means not configured
    case 1:
#ifdef PORT1
      return PORT1;
#else
      return 0;
#endif

    case 2:
#ifdef PORT2
      return PORT2;
#else
      return 0;
#endif

    case 3:
#ifdef PORT3
      return PORT3;
#else
      return 0;
#endif

    case 4:
#ifdef PORT4
      return PORT4;
#else
      return 0;
#endif

    case 5:
#ifdef PORT5
      return PORT5;
#else
      return 0;
#endif

    case 6:
#ifdef PORT6
      return PORT6;
#else
      return 0;
#endif

    case 7:
#ifdef PORT7
      return PORT7;
#else
      return 0;
#endif

    case 8:
#ifdef PORT8
      return PORT8;
#else
      return 0;
#endif

    case 9:
#ifdef PORT9
      return PORT9;
#else
      return 0;
#endif

    default:
      return 0;
  }
}
