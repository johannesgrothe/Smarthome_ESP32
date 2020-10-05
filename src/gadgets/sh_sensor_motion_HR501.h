#pragma once

#include "sh_sensor_motion.h"

class SH_Sensor_Motion_HR501 : public SH_Sensor_Motion {
private:
  int input_pin_;

public:
  /**
   * constructor for class SH_Sensor_Motion_HR501
   * @param gadget
   */
  explicit SH_Sensor_Motion_HR501(const JsonObject &);

  void refresh() override;

};
