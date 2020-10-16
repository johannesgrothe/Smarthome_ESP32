#pragma once

#include "configs/config_test1.h"
//#include "configs/config_test2.h"

static uint8_t getPinForPort(uint8_t port) {
  switch (port) {
    case 0:
#ifdef PORT0
  return PORT0;
#else
  return 0;
#endif

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
