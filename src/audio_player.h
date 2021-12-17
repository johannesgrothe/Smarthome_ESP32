#pragma once

#include <utility>

#include "console_logger.h"
#include "hardware_controller.h"
#include "audio_file.h"


class AudioPlayer {
private:
  uint8_t audio_pin_;
  AudioFile audio_file_;

public:
  AudioPlayer(uint8_t audio_pin, AudioFile audio_file);
  void playAudioFile(uint8_t audio_pin, const AudioFile& audio_file);
};

AudioPlayer::AudioPlayer(uint8_t audio_pin, AudioFile audio_file) :
  audio_pin_(audio_pin),
  audio_file_(std::move(audio_file)){}

void AudioPlayer::playAudioFile(uint8_t audio_pin, const AudioFile& audio_file) {
  std::vector<uint8_t> sound_to_play = audio_file.sound_data_;
  uint64_t sound_sample_rate = ((1 / audio_file.sample_rate_) * 1000 )* 1000;
  HardwareController hdw_ctl;
  for (auto sample: sound_to_play) {
    HardwareController::dacWritePin(audio_pin, sample);
    HardwareController::sleepMicroseconds(sound_sample_rate);
  }
}
