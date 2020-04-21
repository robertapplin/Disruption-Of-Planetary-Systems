#include "ThreadPool.h"

ThreadPool::ThreadPool(std::size_t numberOfThreads) : m_stop(false) {
  for (auto i = 0u; i < numberOfThreads; ++i)
    m_workers.emplace_back([&] {
      for (;;) {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(m_mutex);
          m_condition.wait(lock, [&] { return m_stop || !m_tasks.empty(); });
          if (m_stop && m_tasks.empty())
            return;
          task = std::move(m_tasks.front());
          m_tasks.pop();
        }

        task();
      }
    });
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_stop = true;
  }
  m_condition.notify_all();
  for (auto &worker : m_workers)
    worker.join();
}

void ThreadPool::addToQueue(std::function<void()> const &func) {
  if (m_stop)
    throw std::runtime_error("The ThreadPool has been stopped.");

  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_tasks.emplace(func);
  }
  m_condition.notify_one();
}
