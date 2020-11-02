#include "sh_main.h"
#include "console_logger.h"

SH_Main smarthome_system;

[[noreturn]] static void mainTask(void *args) {
  while (true) {
    smarthome_system.refresh();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

[[noreturn]] static void networkTask(void *args) {
  while (true) {
    smarthome_system.refreshNetwork();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

static void createTasks() {
  TaskHandle_t main_task;
  TaskHandle_t network_task;

  xTaskCreatePinnedToCore(
    mainTask,     /* Task function. */
    "Smarthome_Main",       /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &main_task,
    0);            /* Task handle. */

  xTaskCreatePinnedToCore(
    networkTask,     /* Task function. */
    "Smarthome_MQTT",       /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &network_task,
    1);            /* Task handle. */

  //    vTaskSuspend(main_task);
  //    vTaskResume(main_task);
}

static void startSmarthomeSystem() {
  smarthome_system.init();
  createTasks();
}

void setup() {
  startSmarthomeSystem();
}

void loop() {}
