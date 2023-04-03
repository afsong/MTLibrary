#ifndef job_queue_HPP
#define job_queue_HPP

#include <cstddef>
#include <mutex>
#include <queue>

namespace mt {
    template <typename T> class job_queue {
      public:
        job_queue(size_t cap);

        void add_job(const T &job);
        auto pop_job() -> T;

        [[nodiscard]] auto size() const -> size_t;
        [[nodiscard]] auto cap() const -> size_t;

      private:
        std::queue<T> task_queue;
        std::mutex mutex;
        size_t capacity;
    };

    template <typename T> job_queue<T>::job_queue(size_t cap) : task_queue(), capacity(cap) {}

    template <typename T> void job_queue<T>::add_job(const T &job) {
        std::lock_guard<std::mutex> lock(mutex);

        if (task_queue.size() < capacity) {
            task_queue.push(job);
        }
    }

    template <typename T> auto job_queue<T>::pop_job() -> T {
        std::lock_guard<std::mutex> lock(mutex);
        if (task_queue.size() <= 0) {
            return T();
        }
        T ret = task_queue.front();
        task_queue.pop();

        return ret;
    }

    template <typename T> auto job_queue<T>::size() const -> size_t 
    {
        std::lock_guard<std::mutex> lock(mutex); 
        return task_queue.size(); 
    }

    template <typename T> auto job_queue<T>::cap() const -> size_t { return capacity; }

}  // namespace mt

#endif
