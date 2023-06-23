#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <mutex>
#include <thread>
#include <deque>
#include <functional>
#include <cstdint>
#include <iostream>

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
    std::lock_guard<std::mutex> waitingMutex(waitingMutex);
    waiting = false;
  }

  ~ThreadPool() {
    stop();
    for (std::thread& worker : workers) {
      worker.join();
    }
  }
private:
  std::mutex jobsMutex;
  std::deque<std::function<void()>> jobQueue;
  std::vector<std::thread> workers;
  bool waiting = true;
  std::mutex waitingMutex;

  void waitForWork() {
    while (true) {
      jobsMutex.lock();
      waitingMutex.lock();
      if (jobQueue.empty() && waiting) {
        jobsMutex.unlock();
        waitingMutex.unlock();
        continue;
      }
      if (jobQueue.empty() && !waiting) {
        jobsMutex.unlock();
        waitingMutex.unlock();
        break;
      }
      waitingMutex.unlock();
      auto job = jobQueue.front();
      jobQueue.pop_front();
      jobsMutex.unlock();
      
      job();
    }
  }
};

#endif // !THREAD_POOL_H
