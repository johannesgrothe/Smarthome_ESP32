#pragma once

class SimpleHardwareGadget {
private:
  // Indicates something has changed and the hardware needs to update
  bool needs_hw_update_;

protected:

  /**
   * Sets the internal status to the requested value
   * @param needs_hw_change The value to set the internal status to
   */
  void setHWChangeStatus(bool needs_hw_change);

  /**
   * Sets the internal status to false and then returns the former status
   * @return The status before setting it to false
   */
  bool executeHWChange();

  /**
   * Returns the internal status
   * @return The internal status
   */
  bool needsHWChange() const;

public:
  explicit SimpleHardwareGadget(bool needs_initial_update);
};