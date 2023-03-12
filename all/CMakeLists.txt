# this script adds all subprojects to a single build to allow IDEs understand the full project
# structure.

CMAKE_MINIMUM_REQUIRED(VERSION 3.14...3.22)

PROJECT(BuildAll LANGUAGES CXX)

INCLUDE(../cmake/tools.cmake)

# needed to generate test target
ENABLE_TESTING()

ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/../standalone ${CMAKE_BINARY_DIR}/standalone)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/../test ${CMAKE_BINARY_DIR}/test)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/../documentation ${CMAKE_BINARY_DIR}/documentation)
