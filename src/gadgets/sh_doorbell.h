#pragma once

#include "sh_gadget.h"

/**
 * Class that implements a simple doorbell
 */
class SH_Doorbell : public SH_Gadget {
protected:

  void executeCharacteristicUpdate(CharacteristicIdentifier characteristic, int value) override;

public:

  explicit SH_Doorbell(std::string name);

  /**
   * Registeres a triggered doorbell and updates remote
   */
  void doorbellTriggered();

  vector<GadgetCharacteristicSettings> getCharacteristics() override;

  void handleMethodUpdate(GadgetMethod method) override;
};
