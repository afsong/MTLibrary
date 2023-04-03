#ifndef thread_pool_HPP
#define thread_pool_HPP

#include <stddef.h>

#include <thread>

#include "job_queue.hpp"

namespace mt {
    template <typename job_t, typename func_t> class thread_pool {
      public:
        thread_pool(size_t cap, job_t type, func_t function);
        int submit_job(job_t job);
        bool terminate();

        size_t size;

      private:
        void thread_loop();

        bool should_run = true;

        std::vector<std::thread> worker;
        mt::job_queue<job_t> jq;
        func_t handle;
    };
    template <size_t size, typename job_t, typename func_t>
    thread_pool<size, job_t, func_t>::thread_pool(size cap, job_t job, func_t function)
        : size(cap), handle(function) {
        jp(size);
        for (int i = 0; i < size; i++) {
            worker.push_back(std::thread(handle, i));
        }
    }
    template <size_t size, typename job_t, typename func_t>
    int thread_pool<size, job_t, func_t>::submit_job(job_t job) {
        if (jq.size() < jq.cap()) {
            jq.add_job(job);

            return 0;
        }
        return 1;
    }
    template <size_t size, typename job_t, typename func_t>
    bool thread_pool<size, job_t, func_t>::terminate() {
        should_run = false;
        return !should_run;
    }
    template <size_t size, typename job_t, typename func_t>
    void thread_pool<size, job_t, func_t>::thread_loop() {
        while (true) {
            if (!should_run) {
                // cv.notify_all();
                for (std::thread& active_thread : worker) {
                    active_thread.join();
                }

                worker.clear();
                return;
            }
            job_t j = jq.pop_job();
            if (j) {
                handle(j);
            }
        }
    }

}  // namespace mt

#endif
