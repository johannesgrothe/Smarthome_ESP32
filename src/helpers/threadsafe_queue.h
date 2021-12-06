#pragma once

// Stolen from https://stackoverflow.com/questions/15278343/c11-thread-safe-queue#16075550

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>


/**
 * A Thread-safe queue for instances of std::shared_ptr<Request>
 */
template<class T>
class ThreadSafeQueue {
private:
  std::queue<T> queue_{};
  mutable std::mutex mutex_;
  std::condition_variable condition_var_;

public:
  ThreadSafeQueue() = default;

  /**
   * Adds an Element to the Queue
   * @param req Element to add
   */
  void push(T value) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(value);
    condition_var_.notify_one();
  }

  /**
   * Returns the first element and removes it from the queue.
   * Returns a nullptr if queue is empty
   * @return The first element in the queue
   */
  T pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.empty()) {
      return nullptr;
    }
    auto val = queue_.front();
    queue_.pop();
    return val;
  }

  /**
   * Checks if the queue is empty
   * @return Whether the queue is empty or not
   */
  bool isEmpty() const {
    return queue_.empty();
  }
};
