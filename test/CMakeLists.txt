CMAKE_MINIMUM_REQUIRED(VERSION 3.14...3.22)

PROJECT(mtTests LANGUAGES CXX)

# ---- Options ----

OPTION(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
OPTION(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

INCLUDE(../cmake/tools.cmake)

# ---- Dependencies ----

INCLUDE(../cmake/CPM.cmake)

CPMADDPACKAGE("gh:doctest/doctest@2.4.9")
CPMADDPACKAGE("gh:TheLartians/Format.cmake@1.7.3")

IF(TEST_INSTALLED_VERSION)
    FIND_PACKAGE(mt REQUIRED)
ELSE()
    CPMADDPACKAGE(NAME mt SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
ENDIF()

# ---- Create binary ----

FILE(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
ADD_EXECUTABLE(${PROJECT_NAME} ${sources})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} doctest::doctest mt::mt)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

# enable compiler warnings
IF(NOT TEST_INSTALLED_VERSION)
    IF(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        TARGET_COMPILE_OPTIONS(mt PUBLIC -Wall -Wpedantic -Wextra -Werror)
    ELSEIF(MSVC)
        TARGET_COMPILE_OPTIONS(mt PUBLIC /W4 /WX)
        TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
    ENDIF()
ENDIF()

# ---- Add mtTests ----

ENABLE_TESTING()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME ${PROJECT_NAME} COMMAND
# ${PROJECT_NAME})

INCLUDE(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
DOCTEST_DISCOVER_TESTS(${PROJECT_NAME})

# ---- code coverage ----

IF(ENABLE_TEST_COVERAGE)
    TARGET_COMPILE_OPTIONS(mt PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
    TARGET_LINK_OPTIONS(mt PUBLIC -fprofile-arcs -ftest-coverage)
ENDIF()
