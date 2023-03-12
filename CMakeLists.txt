CMAKE_MINIMUM_REQUIRED(VERSION 3.14...3.22)

# ---- Project ----

# Note: update this to your new project's name and version
PROJECT(
    mt
    VERSION 1.0
    LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 20)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Include guards ----

IF(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    MESSAGE(
        FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
ENDIF()

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

INCLUDE(cmake/CPM.cmake)

# PackageProject.cmake will be used to make our target installable
CPMADDPACKAGE("gh:TheLartians/PackageProject.cmake@1.8.0")

# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
FILE(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
FILE(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

# ---- Create library ----

# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(${PROJECT_NAME} INTERFACE)
ADD_LIBRARY(${PROJECT_NAME} ${headers} ${sources})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

# being a cross-platform target, we enforce standards conformance on MSVC
TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                  $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>)

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths
STRING(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

PACKAGEPROJECT(
    NAME
    ${PROJECT_NAME}
    VERSION
    ${PROJECT_VERSION}
    NAMESPACE
    ${PROJECT_NAME}
    BINARY_DIR
    ${PROJECT_BINARY_DIR}
    INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/include
    INCLUDE_DESTINATION
    include/${PROJECT_NAME}-${PROJECT_VERSION}
    VERSION_HEADER
    "${VERSION_HEADER_LOCATION}"
    COMPATIBILITY
    SameMajorVersion)
