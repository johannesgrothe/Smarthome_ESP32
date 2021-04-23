#include "request_queue.h"

void RequestQueue::push(std::shared_ptr<Request> req) {
  std::lock_guard<std::mutex> lock(mutex_);
  queue_.push(req);
  condition_var_.notify_one();
}

std::shared_ptr<Request> RequestQueue::pop() {
  std::unique_lock<std::mutex> lock(mutex_);
  if (queue_.empty()) {
    return nullptr;
  }
  auto val = queue_.front();
  queue_.pop();
  return val;
}

bool RequestQueue::isEmpty() const {
  return queue_.empty();
}
