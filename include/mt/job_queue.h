#ifndef job_queue_H
#define job_queue_H

#include <cstddef>
#include <mutex>
#include <queue>

namespace Mt {
  template <typename T> class job_queue {
  public:
    job_queue(size_t cap);
    ~job_queue();

    void add_job(const T &job);
    T pop_job();

    size_t size() const;
    size_t cap() const;

  private:
    std::queue<T> task_queue;
    std::mutex mutex;
    size_t capacity;
  };

  template <typename T> job_queue<T>::job_queue(size_t cap)
      : task_queue(), mutex(), capacity(cap) {}

  template <typename T> job_queue<T>::~job_queue() {}

  template <typename T> void job_queue<T>::add_job(const T &job) {
    std::lock_guard<std::mutex> lock(mutex);

    if (task_queue.size() < capacity) {
      task_queue.push(job);
    }
  }

  template <typename T> T job_queue<T>::pop_job() {
    std::lock_guard<std::mutex> lock(mutex);
    if (task_queue.size() <= 0) {
      return T();
    }
    T ret = task_queue.front();
    task_queue.pop();

    return ret;
  }

  template <typename T> size_t job_queue<T>::size() const { return task_queue.size(); }

  template <typename T> size_t job_queue<T>::cap() const { return capacity; }

}  // namespace mt

#endif
