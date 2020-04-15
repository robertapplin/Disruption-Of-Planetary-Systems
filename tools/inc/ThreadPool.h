#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

class ThreadPool {
public:
  ThreadPool(std::size_t numberOfThreads);
  ~ThreadPool();

  void addToQueue(std::function<void()> const &func);

private:
  // Need to keep track of threads so we can join them
  std::vector<std::thread> m_workers;
  // The task queue
  std::queue<std::function<void()>> m_tasks;

  // Synchronization
  std::mutex m_mutex;
  std::condition_variable m_condition;
  bool m_stop;
};

#endif /* THREAD_POOL_H */
