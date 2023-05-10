#include <doctest/doctest.h>

#include <circular_buffer.hpp>

TEST_CASE("TestCircularBufferConstructor") {
    mt::circular_buffer<char> buffer(1000);
    CHECK_EQ(buffer.size(), 0);
    CHECK_EQ(buffer.cap(), 1000);
}

TEST_CASE("TestCircularBufferInsertEntry") {
    mt::circular_buffer<char> buffer(1000);

    buffer.add_entry('a');
    CHECK_EQ(buffer.size(), 1);
}

TEST_CASE("TestCircularBufferGetEntry") {
    mt::circular_buffer<char> buffer(1000);

    buffer.add_entry('a');
    CHECK_EQ(buffer.size(), 1);

    CHECK_EQ('a', buffer.get_entry());
    CHECK_EQ(buffer.size(), 0);
}
