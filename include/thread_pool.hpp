#ifndef thread_pool_HPP
#define thread_pool_HPP

#include <atomic>
#include <cstddef>
#include <iostream>
#include <job_queue.hpp>
#include <thread>

namespace mt {
    /**
     * @brief a generic thread pool that continuously waits for jobs from the job queue and
     * processes them
     *
     * @tparam JOB the type of JOB that is transferred from the producer to thread pool
     * @tparam FUNC the type of function that will be used to handle the jobs
     */
    template <typename JOB, typename FUNC> class thread_pool {
      public:
        static constexpr size_t DEF_NUM_THREADS = 4;

        /**
         * @brief Construct a new thread pool object
         *
         * @param cap capacity of the underlying job queue
         * @param func function handler that will be used to process the jobs
         */
        thread_pool(size_t cap, FUNC func);

        /**
         * @brief Destroy the thread pool object
         *
         */
        ~thread_pool();

        /**
         * @brief Submit a new job to the thread pool
         *
         * @param job job to be submitted
         * @return true if job was successfully submitted
         * @return false if failed to submit job
         */
        auto submit_job(const JOB& job) -> bool;

        /**
         * @brief Terminate the thread pool
         *
         * @return true if successfully terminated the pool
         * @return false if failed to terminate the pool (most likely because it was already
         * terminated)
         */
        auto terminate() -> bool;

        /**
         * @brief Copy constructor is deleted
         */
        thread_pool(const thread_pool& pool) = delete;

        /**
         * @brief Move constructor is deleted
         */
        thread_pool(thread_pool&& pool) = delete;

        /**
         * @brief copy assignment operator is deleted
         */
        auto operator=(const thread_pool& pool) -> thread_pool& = delete;

        /**
         * @brief move assignment operator is deleted
         */
        auto operator=(thread_pool&& pool) -> thread_pool& = delete;

      private:
        void thread_loop();

        std::atomic<bool> flg_active;
        std::vector<std::thread> workers;
        mt::job_queue<JOB> jq;
        FUNC handle;
    };

    template <typename JOB, typename FUNC>
    thread_pool<JOB, FUNC>::thread_pool(size_t cap, FUNC func)
        : flg_active(true), jq(cap), handle(func) {
        for (size_t i = 0; i < DEF_NUM_THREADS; i++) {
            workers.emplace_back(&thread_pool<JOB, FUNC>::thread_loop, this);
        }
    }

    template <typename JOB, typename FUNC> thread_pool<JOB, FUNC>::~thread_pool() {
        terminate();
        jq.stop_queue();

        for (size_t i = 0; i < DEF_NUM_THREADS; ++i) {
            workers[i].join();
        }
    }

    template <typename JOB, typename FUNC> auto thread_pool<JOB, FUNC>::submit_job(const JOB& job)
        -> bool {
        return jq.add_job(job);
    }

    template <typename JOB, typename FUNC> auto thread_pool<JOB, FUNC>::terminate() -> bool {
        if (flg_active) {
            flg_active = false;
            return true;
        }

        return false;
    }

    template <typename JOB, typename FUNC> void thread_pool<JOB, FUNC>::thread_loop() {
        while (flg_active) {
            JOB j = jq.pop_job();

            if (j) {
                handle(j);
            }
        }
    }

}  // namespace mt

#endif
