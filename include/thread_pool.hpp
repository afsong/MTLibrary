#ifndef thread_pool_HPP
#define thread_pool_HPP

#include <atomic>
#include <cstddef>
#include <job_queue.hpp>
#include <thread>

namespace mt {
    template <typename JOB, typename FUNC> class thread_pool {
      public:
        static constexpr size_t DEF_NUM_THREADS = 4;

        thread_pool(size_t cap, FUNC func);
        auto submit_job(const JOB& job) -> bool;
        bool terminate();

      private:
        void thread_loop();

        std::atomic<bool> flg_active;
        size_t queue_cap;
        std::vector<std::jthread> workers;
        mt::job_queue<JOB> jq;
        FUNC handle;
    };

    template <typename JOB, typename FUNC>
    thread_pool<JOB, FUNC>::thread_pool(size_t cap, FUNC func)
        : flg_active(false), workers(), jq(cap), handle(func) {
        for (size_t i = 0; i < DEF_NUM_THREADS; i++) {
            workers.emplace_back(std::jthread(handle));
        }
    }

    template <typename JOB, typename FUNC> auto thread_pool<JOB, FUNC>::submit_job(const JOB& job)
        -> bool {
        return jq.add_job(job);
    }

    template <typename JOB, typename FUNC> bool thread_pool<JOB, FUNC>::terminate() {
        if (flg_active) {
            flg_active = false;

            for (auto& thr : workers) {
                thr.request_stop();
            }

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
