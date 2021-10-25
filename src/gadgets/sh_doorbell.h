#pragma once

#include "gadget.h"

/**
 * Class that implements a simple doorbell
 */
class SH_Doorbell : public Gadget {
protected:

  void executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) override;

public:

  explicit SH_Doorbell(std::string name);

  /**
   * Registeres a triggered doorbell and updates remote
   */
  void doorbellTriggered();
};
