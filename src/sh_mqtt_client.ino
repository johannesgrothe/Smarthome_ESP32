#include "sh_main.h"

SH_Main shMain;

void setup() {
  Serial.begin(115200);

  shMain.init();
}

void loop() {
  shMain.refresh();
}
