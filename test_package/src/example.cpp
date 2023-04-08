
#include <job_queue.hpp>

auto main() -> int {
    constexpr size_t CAP = 100;

    mt::job_queue<int> jq(CAP);
    return 0;
}