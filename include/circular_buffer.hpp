#ifndef circular_buffer_HPP
#define circular_buffer_HPP

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>

namespace mt {
    /**
     * @brief A generic circular buffer implementation. Similar to the job
     * queue, it can be used to implement the consumer-producer pattern, but
     * this implementation is cache-friendlier since we use a contiguous array
     * as our data store.
     *
     * @tparam T, tye type of entry that is transferred between the consumers
     * and producers.
     */
    template <typename T> class circular_buffer {
      public:
        /**
         * @brief Construct a new circular buffer object
         *
         * @param cap capacity of the circular buffer
         */
        circular_buffer(size_t cap);

        /**
         * @brief Add a new entry to the buffer
         *
         * @param entry new entry to be placed into the buffer
         * @return true if entry was successfully added to the buffer
         * @return false if entry failed to be added
         */
        auto add_entry(const T& entry) -> bool;

        /**
         * @brief Get an entry from the buffer
         *
         * @return T is the entry returned. It's empty if failed to get entry
         */
        auto get_entry() -> T;

        /**
         * @brief Mark the buffer as inactive and notify all waiters
         *
         */
        void stop_buffer();

        /**
         * @brief Return the number of entries in the buffer
         *
         * @return size_t, the size of the queue
         */
        [[nodiscard]] auto size() const -> size_t;

        /**
         * @brief Return the capacity of the buffer
         *
         * @return size_t, the capacity of the buffer
         */
        [[nodiscard]] auto cap() const -> size_t;

      private:
        auto is_full() const -> bool;
        auto is_empty() const -> bool;

        std::vector<T> buffer;
        mutable std::mutex mutex;
        std::condition_variable cond;
        size_t capacity;
        bool flg_active;

        size_t head;
        size_t tail;
    };

    template <typename T> circular_buffer<T>::circular_buffer(size_t cap)
        : buffer(cap + 1), capacity(cap + 1), flg_active(true), head(0), tail(0) {}

    template <typename T> auto circular_buffer<T>::add_entry(const T& entry) -> bool {
        std::lock_guard<std::mutex> lock(mutex);

        if (!flg_active) {
            return false;
        }

        if (this->is_full()) {
            return false;
        }

        buffer[tail] = entry;
        tail = (tail + 1) % capacity;

        cond.notify_one();

        return true;
    }

    template <typename T> auto circular_buffer<T>::get_entry() -> T {
        std::unique_lock<std::mutex> lock(mutex);

        cond.wait(lock, [this]() { return !flg_active || !this->is_empty(); });

        if (!flg_active) {
            return T();
        }

        T ret = buffer[head];

        head = (head + 1) % capacity;

        return ret;
    }

    template <typename T> void circular_buffer<T>::stop_buffer() {
        std::lock_guard<std::mutex> lock(mutex);
        flg_active = false;
        cond.notify_all();
    }

    template <typename T> auto circular_buffer<T>::size() const -> size_t {
        std::lock_guard<std::mutex> lock(mutex);

        if (tail >= head) {
            return tail - head;
        }

        return tail + capacity - head;
    }

    template <typename T> auto circular_buffer<T>::cap() const -> size_t { return capacity - 1; }

    // private function, must be called with the mutex locked
    template <typename T> auto circular_buffer<T>::is_full() const -> bool {
        return ((head + 1) % capacity) == tail;
    }

    // private function, must be called with the mutex locked
    template <typename T> auto circular_buffer<T>::is_empty() const -> bool { return head == tail; }

}  // namespace mt

#endif
