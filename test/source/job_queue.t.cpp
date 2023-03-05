#include <mt/job_queue.h>
#include <doctest/doctest.h>

TEST_CASE("TestJobQueueConstructor")
{
    mt::job_queue<int> jq(100);

    CHECK_EQ(jq.size(), 0);
    CHECK_EQ(jq.cap(), 100);
}


TEST_CASE("TestJobQueueAdd"){
    mt::job_queue<int> jq(100);

    jq.add_job(5);
    CHECK_EQ(jq.size(),1);
    jq.add_job(10);
    CHECK_EQ(jq.size(),2);

}
TEST_CASE("TestJobQueueAddAboveCap"){
    mt::job_queue<int> jq(100);
    for (int i =0;i<100;i++){
        jq.add_job(1);
    }
    jq.add_job(2);
    CHECK_EQ(jq.size(),100);
    
}

TEST_CASE("TestJobQueuePop"){
    mt::job_queue<int> jq(100);

    jq.add_job(5);
    jq.add_job(10);
    int num = jq.pop_job();
    CHECK_EQ(num,5);
    
}
TEST_CASE("TestJobQueuePopEmptyQueue"){
    mt::job_queue<int> jq(100);

    jq.add_job(5);
    int num = jq.pop_job();
    CHECK_EQ(num,5);

    num = jq.pop_job();
    CHECK_EQ(num,0);
    CHECK_EQ(jq.size(),0);
}