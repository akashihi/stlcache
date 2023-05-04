#Preamble
cmake_minimum_required(VERSION 3.15)

# Detect if we are bundled and skip building of tests in that case
IF (NOT DEFINED PROJECT_NAME)
    SET(NOT_SUBPROJECT ON)
ENDIF ()

project(STLCache)

#Environment detection
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#Check for libraries presence
FIND_PACKAGE(Boost 1.58.0)

#Documentation stuff
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include(UseDoxygen)
set(DOXYFILE_LATEX OFF)

#Build configuration
include_directories(${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/include
        )

install(DIRECTORY include/stlcache DESTINATION include)

IF (NOT_SUBPROJECT)
    # Set standard
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    # Compile Boost specific tests
    if (Boost_FOUND)
        add_compile_definitions(USE_BOOST)
        include_directories(${Boost_INCLUDE_DIRS})
    endif ()

    # Retrieve the Catch2
    Include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.3.2
    )
    FetchContent_MakeAvailable(Catch2)

    # Retrieve Google Benchmark
    SET(BENCHMARK_ENABLE_TESTING OFF)
    SET(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
    FetchContent_Declare(
            Benchmark
            GIT_REPOSITORY https://github.com/google/benchmark.git
            GIT_TAG v1.7.1
    )
    FetchContent_MakeAvailable(Benchmark)

    # Unit tests binary
    add_executable(tests
            tests/test_main.cpp tests/test_map.cpp tests/test_map_interface.cpp tests/test_iterator.cpp tests/test_optional.cpp tests/test_insert_assign.cpp
            tests/test_victim.cpp tests/test_policy.cpp
            tests/test_lru.cpp tests/test_mru.cpp
            tests/test_lfu.cpp tests/test_lfustar.cpp tests/test_lfuaging.cpp tests/test_lfuagingstar.cpp tests/test_lfu_multi.cpp
            tests/test_adaptive.cpp)
    target_link_libraries(tests PRIVATE Catch2::Catch2WithMain ${LIBS})

    # Add tests auto discovery
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(CTest)
    include(Catch)
    enable_testing()
    catch_discover_tests(tests)

    # Benchmarks
    add_executable(insert_perf benchmarks/insert_perf.cpp)
    target_link_libraries(insert_perf benchmark::benchmark)

    add_executable(insdel_perf benchmarks/insdel_perf.cpp)
    target_link_libraries(insdel_perf benchmark::benchmark)

    add_executable(eviction_perf benchmarks/eviction_perf.cpp)
    target_link_libraries(eviction_perf benchmark::benchmark)
ENDIF ()
