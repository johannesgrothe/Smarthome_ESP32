#pragma once

//#include ""

// All possible gadget types
enum class GadgetIdentifier {
  err_type = 0,
  lamp_neopixel_basic,
  lamp_basic,
  fan_westinghouse_ir,
  lamp_westinghouse_ir,
  doorbell_basic,
  wallswitch_basic,
  sensor_motion_hr501,
  sensor_temperature_dht
};

// Count of the gadgets
#define GadgetIdentifierCount 5

// All GadgetMethods used to update the gadget status
enum class GadgetMethod {
  err_type = 0,
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
enum class CharacteristicIdentifier {
  err_type = 0,
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
  const CharacteristicIdentifier characteristic;
  const int min;
  const int max;
  const int step;
  const long value;

  GadgetCharacteristicSettings(CharacteristicIdentifier characteristic, int min, int max, int step, long value):
  characteristic(characteristic),
  min(min),
  max(max),
  step(step),
  value(value) {}
};

// Count of the gadget characteristics
#define GadgetCharacteristicCount 7

static CharacteristicIdentifier getCharacteristicIdentifierFromInt(int char_value) {
  if (char_value > 1 && char_value < GadgetCharacteristicCount) {
    return CharacteristicIdentifier(char_value);
  }
  return CharacteristicIdentifier::err_type;
}
