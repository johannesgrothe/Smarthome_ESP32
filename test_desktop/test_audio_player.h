#include "catch.hpp"

#include "../src/audio_player.h"
#include <vector>

TEST_CASE("Test Audio Player", "") {
  SECTION("Test Audio Player initial") {
    std::vector<uint8_t> file(500, 0);
    for(uint8_t i = 0; i <= 500; i++) {
      auto val = i % 2;
      val == 1 ? file[i] += val : file[i] -= val
    }
    AudioFile audio_file = new AudioFile(file, 60);
    AudioPlayer player = new AudioPlayer(25, file);
    player.playAudioFile()
  }
}
