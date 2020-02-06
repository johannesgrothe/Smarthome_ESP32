#include "sh_client.h"
#include "sh_main.h"
#include "colors.h"

SH_Main shMain;

void setup() {
  Serial.begin(115200);

  shMain.init();
}

void loop() {
  shMain.refresh();
  delay(3000);
}
