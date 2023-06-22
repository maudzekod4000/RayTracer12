#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <mutex>
#include <thread>
#include <deque>
#include <functional>
#include <cstdint>

struct ThreadPool {
  inline void start() {
    int32_t coreCount = std::thread::hardware_concurrency();
    workers.reserve(coreCount);
    for (int32_t i = 0; i < coreCount; i++) {
      workers.push_back(std::thread(&ThreadPool::waitForWork, this));
    }
  }

  inline void doJob(std::function<void()> job) {
    std::lock_guard<std::mutex> jobQueueGuard(jobsMutex);
    jobQueue.push_back(job);
  }

  inline void stop() {
    working = false;
  }
private:
  std::mutex jobsMutex;
  std::deque<std::function<void()>> jobQueue;
  std::vector<std::thread> workers;
  bool working = true;

  void waitForWork() {
    while (working) {
      std::lock_guard<std::mutex> jobQueueGuard(jobsMutex);
      if (jobQueue.empty()) { continue; }
      auto job = jobQueue.front();
      jobQueue.pop_front();
      job();
    }
  }
};

#endif // !THREAD_POOL_H
