#include <mt/job_queue.h>
#include <doctest/doctest.h>

TEST_CASE("TestJobQueueConstructor")
{
    mt::job_queue<int> jq(100);

    CHECK_EQ(jq.size(), 0);
    CHECK_EQ(jq.cap(), 100);
}