#include "sh_main.h"

SH_Main shMain;

void setup() {
  shMain.init();
}

void loop() {
  shMain.refresh();
}
