#ifndef job_queue_HPP
#define job_queue_HPP

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>

namespace mt {
    /**
     * @brief A generic job queue structure that implements the classical consumer-producer pattern.
     * It's thread-safe and can be used to transfer any type.
     *
     * @subsection job_queue Example
     * @code{.cpp}
     * #include <job_queue.hpp>
     *
     * // at most 100 integers can be placed on the queue
     * mt::job_queue<int> jq(100);
     *
     * // in some producer thread
     * jq.add_job(1);
     *
     * // in some consumer thread
     * jq.pop_job();
     *
     * // to signal that a job queue is inactive and no more threads should use it
     * jq.stop_queue();
     *
     * @endcode
     *
     *
     * @tparam T, the type of "job" that is transferred between the consumers and producers
     *
     *
     */
    template <typename T> class job_queue {
      public:
        /**
         * @brief Construct a new job queue object
         *
         * @param cap capacity of the job queue
         */
        job_queue(size_t cap);

        /**
         * @brief Add a new job to the queue
         *
         * @param job new job to be placed onto the queue
         * @return true if job was successfully added to the queue
         * @return false if job failed to be added (queue is not active or queue is full)
         */
        auto add_job(const T &job) -> bool;

        /**
         * @brief Pop a job from the queue
         *
         * @return T is the job returned. It's empty if failed to pop job
         */
        auto pop_job() -> T;

        /**
         * @brief Mark the queue as inactive and notify all waiters
         *
         */
        void stop_queue();

        /**
         * @brief  Return the size of the queue
         *
         * @return size_t, the size of the queue
         */
        [[nodiscard]] auto size() const -> size_t;

        /**
         * @brief Return the capacity of the queue
         *
         * @return size_t, the capacity of the queue
         */
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
