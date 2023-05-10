#include <doctest/doctest.h>

#include <functional>
#include <thread_pool.hpp>

std::atomic<int> count;

namespace {

    using job_t = int;
    using job_handler_t = std::function<bool(int)>;

    bool is_prime(int x) {
        if (x <= 1) {
            return false;
        }

        for (int i = 2; i * i <= x; ++i) {
            if (x % i == 0) {
                return false;
            }
        }

        count++;
        return true;
    }

}  // namespace

TEST_CASE("TestThreadPoolConstructor") {
    mt::thread_pool<job_t, job_handler_t> pool(100, &is_prime);
    CHECK_EQ(1, 1);
}

TEST_CASE("TestThreadPoolEnqueueJobs") {
    count = 0;

    mt::thread_pool<job_t, job_handler_t> pool(10000, &is_prime);

    for (int i = 1; i <= 10000; ++i) {
        CHECK(pool.submit_job(i));
    }

    // sleep for one second to wait while the job queue is finished
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    CHECK_EQ(count.load(), 1229);
}
