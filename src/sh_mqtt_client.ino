#include "sh_main.h"
#include "console_logger.h"

SH_Main sh_main;

void networkTask(void *args) {
  while (true) {
    sh_main.refreshNetworks();
  }
}

void setup() {
  sh_main.init();
  logger.println("Starting Network Task...");
  xTaskCreatePinnedToCore(
    networkTask,     /* Task function. */
    "Consumer",       /* String with name of task. */
    100000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL,
    1);            /* Task handle. */
}

void loop() {
  sh_main.refresh();
//  sh_main.refreshNetworks();
}
