#pragma once

#include "sh_gadget.h"

/**
 * Class that implements a simple doorbell
 */
class SH_Doorbell : public SH_Gadget {
protected:

public:

  explicit SH_Doorbell(std::string name);

  /**
   * Registeres a triggered doorbell and updates remote
   */
  void doorbellTriggered();

  void handleCharacteristicUpdate(const char *characteristic, int value) override;

  bool getCharacteristics(char *buffer) override;

  void handleMethodUpdate(GadgetMethod method) override;
};
