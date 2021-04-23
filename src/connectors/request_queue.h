// Stolen from https://stackoverflow.com/questions/15278343/c11-thread-safe-queue#16075550
#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "request.h"

/**
 * A Thread-safe queue for instances of std::shared_ptr<Request>
 */
class RequestQueue {
private:
  std::queue<std::shared_ptr<Request>> queue_{};
  mutable std::mutex mutex_;
  std::condition_variable condition_var_;

public:
  RequestQueue() = default;

  /**
   * Adds an Element to the Queue
   * @param req Element to add
   */
  void push(std::shared_ptr<Request> req);

  /**
   * Returns the first element and removes it from the queue.
   * Returns a nullptr if queue is empty
   * @return The first element in the queue
   */
  std::shared_ptr<Request> pop();
};