# this file contains a list of tools that can be activated and downloaded on-demand each tool is
# enabled during configuration by passing an additional `-DUSE_<TOOL>=<VALUE>` argument to CMake

# only activate tools for top level project
IF(NOT PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    RETURN()
ENDIF()

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

# enables sanitizers support using the the `USE_SANITIZER` flag available values are: Address,
# Memory, MemoryWithOrigins, Undefined, Thread, Leak, 'Address;Undefined'
IF(USE_SANITIZER OR USE_STATIC_ANALYZER)
    CPMADDPACKAGE("gh:StableCoder/cmake-scripts#1f822d1fc87c8d7720c074cde8a278b44963c354")

    IF(USE_SANITIZER)
        INCLUDE(${cmake-scripts_SOURCE_DIR}/sanitizers.cmake)
    ENDIF()

    IF(USE_STATIC_ANALYZER)
        IF("clang-tidy" IN_LIST USE_STATIC_ANALYZER)
            SET(CLANG_TIDY
                ON
                CACHE INTERNAL "")
        ELSE()
            SET(CLANG_TIDY
                OFF
                CACHE INTERNAL "")
        ENDIF()
        IF("iwyu" IN_LIST USE_STATIC_ANALYZER)
            SET(IWYU
                ON
                CACHE INTERNAL "")
        ELSE()
            SET(IWYU
                OFF
                CACHE INTERNAL "")
        ENDIF()
        IF("cppcheck" IN_LIST USE_STATIC_ANALYZER)
            SET(CPPCHECK
                ON
                CACHE INTERNAL "")
        ELSE()
            SET(CPPCHECK
                OFF
                CACHE INTERNAL "")
        ENDIF()

        INCLUDE(${cmake-scripts_SOURCE_DIR}/tools.cmake)

        CLANG_TIDY(${CLANG_TIDY_ARGS})
        INCLUDE_WHAT_YOU_USE(${IWYU_ARGS})
        CPPCHECK(${CPPCHECK_ARGS})
    ENDIF()
ENDIF()

# enables CCACHE support through the USE_CCACHE flag possible values are: YES, NO or equivalent
IF(USE_CCACHE)
    CPMADDPACKAGE("gh:TheLartians/Ccache.cmake@1.2.3")
ENDIF()
