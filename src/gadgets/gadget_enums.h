#pragma once

// All possible gadget types
enum class GadgetIdentifier {
  None = 0,
  sh_lamp_neopixel_basic,
  sh_lamp_basic,
  sh_fan_westinghouse_ir,
  sh_lamp_westinghouse_ir,
  sh_doorbell_basic,
  sh_wallswitch_basic,
  sh_sensor_motion_hr501,
  sh_sensor_temperature_dht
};

// Count of the gadgets
#define GadgetIdentifierCount 5

// All GadgetMethods used to update the gadget status
enum class GadgetMethod {
  None = 0,
  turnOn,
  turnOff,
  toggleStatus,
  brightnessUp,
  brightnessDown,
  volumeUp,
  volumeDown,
  mute,
  unmute,
  toggleMute,
  mode0,
  mode1,
  mode2,
  mode3,
  mode4,
  modeUp,
  modeDown
};

// Count of the gadget methods
#define GadgetMethodCount 17

// All gadget characteristics
enum class GadgetCharacteristic {
  None = 0,
  status,
  fanSpeed,
  brightness,
  hue,
  saturation,
  temperature,
  humidity
};

class GadgetCharacteristicSettings {
public:
  const GadgetCharacteristic characteristic;
  const int min;
  const int max;
  const int step;
  const long value;

  GadgetCharacteristicSettings(GadgetCharacteristic characteristic, int min, int max, int step, long value):
  characteristic(characteristic),
  min(min),
  max(max),
  step(step),
  value(value) {}
};

// Count of the gadget characteristics
#define GadgetCharacteristicCount 7

static GadgetCharacteristic getCharacteristicFromInt(int char_value) {
  if (char_value > 1 && char_value < GadgetCharacteristicCount) {
    return GadgetCharacteristic(char_value);
  }
  return GadgetCharacteristic::None;
}
