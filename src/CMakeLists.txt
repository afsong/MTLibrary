
add_library(mt job_queue.cpp)

# PROJECT_SOURCE_DIR - macro for project root dir
target_include_directories(mt PUBLIC ${PROJECT_SOURCE_DIR}/include)
