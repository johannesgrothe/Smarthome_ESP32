#pragma once

#include "console_logger.h"

class AudioFile {
private:
  std::vector<uint8_t> sound_data_;
  uint64_t sample_rate_;

public:
  AudioFile(std::vector<uint8_t> data, uint64_t sample_rate);
};

AudioFile::AudioFile(std::vector<uint8_t> data, uint64_t sample_rate) :
  sound_data_(data),
  sample_rate_(sample_rate){}
