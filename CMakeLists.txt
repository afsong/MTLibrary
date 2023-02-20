cmake_minimum_required(VERSION 3.14)
project(mt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

add_subdirectory(driver)
add_subdirectory(src)
add_subdirectory(test)
