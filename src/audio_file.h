#pragma once

#include "console_logger.h"
#include <utility>
#include <vector>

class AudioFile {
public:
  const std::vector<uint8_t> sound_data_;
  const uint64_t sample_rate_;

  AudioFile(std::vector<uint8_t> sound_data, uint64_t sample_rate);
};

AudioFile::AudioFile(std::vector<uint8_t> sound_data, uint64_t sample_rate) :
  sound_data_(std::move(sound_data)),
  sample_rate_(sample_rate){}

