#pragma once

#include <cstring>
#include <functional>
#include <vector>
#include <memory>
#include "../mapping_reference.h"
#include "../system_settings.h"
#include "../datatypes.h"
#include "../console_logger.h"

#include "../connectors/ir_gadget.h"
#include "../connectors/radio_gadget.h"
#include "gadget_characteristic_settings.h"
#include "../connectors/event.h"
#include "../hardware_controller.h"
#include "characteristic.h"


// List of all Gadget Types
enum class GadgetType {
  None, Lightbulb, Fan, Doorbell, Wallswitch, Motion_Sensor, Temp_Humm_Sensor
};

enum SH_RGB_Color {
  SH_CLR_red, SH_CLR_green, SH_CLR_blue
};

/**
 * Base class for all gadgets in the system
 */
class Gadget {
private:

  // stores whether initializing was successful
  bool init_error;

  // Name of the gadget
  const std::string name;

  // Whether the status of the gadget has changed since the last refresh
  bool has_changed;

  // The IR gadget to be used
  std::shared_ptr<IR_Gadget> ir_gadget_;

  // The radio gadget to be used
  std::shared_ptr<Radio_Gadget> radio_gadget_;

  // Maps event to characteristic changes that should be performed in response
  std::vector<gadget_event_map> event_mapping_;

  // All characteristics of the gadget
  std::vector<Characteristic> characteristics_;

protected:

  // Type of the gadget
  const GadgetType type;

  /**
   * Returns the step value for the selected characteristic
   * @param characteristic Characteristic to get value for
   * @return The current step value of this characteristic
   */
  uint16_t getCharacteristicValue(CharacteristicIdentifier characteristic);

  /**
   * Sets the step value for the selected characteristic
   * @param characteristic Characteristic to set value for
   * @param step_value Step value to set
   * @return True if a characteristic has changed, False if nothing has changed or characteristic does not exist
   */
  bool setCharacteristicValue(CharacteristicIdentifier characteristic, uint16_t step_value);

  /**
   * Updates the initialization error. If status_update is true, the error will not change. If its false, the error will now be false.
   * @param status_update The update for the init_error
   */
  void updateInitStatus(bool status_update);

  /**
   * Sends an array of raw IR data
   * @param raw_data Raw IR data
   * @param com_len Length of the ir data array
   * @return Whether sending was successful
   */
  bool sendRawIR(const uint16_t raw_data[], uint8_t com_len);

  /**
   * Sends a IR command
   * @param command Command to send
   * @param com_type Coding type of the ri command
   * @return Whether sending was successful
   */
  bool sendIR(unsigned long command, uint8_t com_type);

  /**
   * Updates the gadget with a characteristic
   * @param characteristic Characteristic to update
   * @param step_value The step-value to update with
   */
  virtual void executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) = 0;

public:

  /**
   * Constructor for the base gadget
   * @param name Name of the gadget. Should be unique in the system.
   * @param type Type of the gadget
   */
  explicit Gadget(std::string name, GadgetType type, std::vector<Characteristic> characteristics);

  /**
   * Returns the type of the gadget
   * @return the type of the gadget
   */
  GadgetType getType();

  /**
   * Returns the unique name of the gadget
   * @return the name of the gadget
   */
  std::string getName() const;

  /**
   * Returns the event mapping configuration for the gadget
   * @return The event mapping configuration for the gadget
   */
  std::vector<gadget_event_map> getMapping() const;

  /**
   * Sets the gadget event mapping
   * @param event_mapping Event mapping data
   */
  void setMapping(std::vector<gadget_event_map> event_mapping);

  /**
   * Returns all of the characteristics the gadget makes use of
   * @param characteristic_str [out] a string containing all characteristics
   * @return whether writing the characteristics to the string was successful
   */
  std::vector<GadgetCharacteristicSettings> getCharacteristics();

  /**
   * Returns the data for the selected characteristic
   * @param identifier Characteristic to get
   * @return The characteristic data
   */
  std::shared_ptr<GadgetCharacteristicSettings> getCharacteristic(CharacteristicIdentifier identifier);

  /**
   * Checks if the gadget was successfully initialized
   * @return whether the gadget was successfully initialized
   */
  bool hasInitError() const;

  /**
   * Checks if the gadget has changed since the last refresh
   * @return Whether the gadget has changed since the last refresh
   */
  bool hasChanged();

  /**
   * Updates the gadget with a characteristic
   * @param characteristic Characteristic the gadget should be updated with
   * @param step_value The value the characteristic should be updated with
   */
  virtual bool handleCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value);

  /**
   * Handles and changes the gadget based on an Event triggered by any other system component
   * @param event_id Identifier string of the event
   */
  virtual bool handleEvent(const std::string &event_id);

  /**
   * Refresh the gadget and its hardware. Used as loop method.
   */
  virtual void refresh() = 0;

  /**
   * Initialized the IR capabilities of the gadget
   * @param new_ir_gadget The IR_Gadget to be used
   */
  void setIR(const std::shared_ptr<IR_Gadget> &new_ir_gadget);

  /**
   * Returns whether the gadget has IR access or not
   * @return Whether the gadget has IR access or not
   */
  bool hasIR() const;

  /**
   * Initialized the radio capabilities of the gadget
   * @param new_radio_gadget The Radio_Gadget to be used
   */
  void setRadio(const std::shared_ptr<Radio_Gadget> &new_radio_gadget);

  /**
   * Returns whether the gadget has radio access or not
   * @return Whether the gadget has radio access or not
   */
  bool hasRadio() const;
};
