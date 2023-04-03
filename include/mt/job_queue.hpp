#ifndef job_queue_HPP
#define job_queue_HPP

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>

namespace mt {
    template <typename T> class job_queue {
      public:
        job_queue(size_t cap);

        auto add_job(const T &job) -> bool;
        auto pop_job() -> T;
        void stop_queue();

        [[nodiscard]] auto size() const -> size_t;
        [[nodiscard]] auto cap() const -> size_t;

      private:
        std::queue<T> task_queue;
        mutable std::mutex mutex;
        std::condition_variable cond;
        size_t capacity;
        bool flg_active;
    };

    template <typename T> job_queue<T>::job_queue(size_t cap)
        : task_queue(), capacity(cap), flg_active(true) {}

    template <typename T> auto job_queue<T>::add_job(const T &job) -> bool {
        std::lock_guard<std::mutex> lock(mutex);

        if (!flg_active) {
            return false;
        }

        if (task_queue.size() < capacity) {
            task_queue.push(job);
            cond.notify_one();
            return true;
        }

        return false;
    }

    template <typename T> auto job_queue<T>::pop_job() -> T {
        std::unique_lock<std::mutex> lock(mutex);

        cond.wait(lock, [this]() { return !flg_active || task_queue.size() > 0; });

        if (!flg_active) {
            return T();
        }

        T ret = task_queue.front();
        task_queue.pop();

        return ret;
    }

    template <typename T> void job_queue<T>::stop_queue() {
        std::lock_guard<std::mutex> lock(mutex);
        flg_active = false;
        cond.notify_all();
    }

    template <typename T> auto job_queue<T>::size() const -> size_t {
        std::lock_guard<std::mutex> lock(mutex);
        return task_queue.size();
    }

    template <typename T> auto job_queue<T>::cap() const -> size_t { return capacity; }

}  // namespace mt

#endif
