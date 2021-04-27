#pragma once

#include <Arduino.h>
#include "console_logger.h"

class MainSystemController {
private:
  TaskHandle_t network_task_;
  TaskHandle_t heartbeat_task_;

public:
  MainSystemController(TaskHandle_t network_task, TaskHandle_t heartbeat_task) :
      network_task_(network_task),
      heartbeat_task_(heartbeat_task) {}

  /**
   * Pauses all tasks except the main task. Only usw when necessary!
   */
  void pause_all_tasks_except_main() {
    logger.sender("MainSystemController") << "Pausing all tasks except main...\n";
//    vTaskSuspend(network_task_);
//    vTaskResume(heartbeat_task_);
  }

  /**
   * Resumes all tasks on the system
   */
  void resume_all_tasks() {
    logger.sender("MainSystemController") << "Resuming all tasks...\n";
//    vTaskResume(network_task_);
//    vTaskResume(heartbeat_task_);
  }

};