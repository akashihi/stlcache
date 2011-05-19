#Preamble
cmake_minimum_required (VERSION 2.6)
project (STL::Cache)

#Environment detection
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#Check for libraries presence
SET(BOOST_ROOT "D:\\Boost")
SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost 1.42.0 COMPONENTS system unit_test_framework)

#Documentation stuff
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include(UseDoxygen)
set(DOXYFILE_LATEX OFF)

#Build configuration
include_directories(${CMAKE_BINARY_DIR}
		    ${CMAKE_SOURCE_DIR}/include
		    ${Boost_INCLUDE_DIRS}
)

if(Boost_FOUND)
enable_testing()
SET(CTEST_ENVIRONMENT "BOOST_TEST_LOG_LEVEL=message")

ADD_EXECUTABLE(test_map tests/test_map.cpp)
target_link_libraries(test_map ${Boost_LIBRARIES})
ADD_TEST(Map test_map)

ADD_EXECUTABLE(test_victim tests/test_victim.cpp)
target_link_libraries(test_victim ${Boost_LIBRARIES})
ADD_TEST(Victim test_victim)

ADD_EXECUTABLE(test_policy tests/test_policy.cpp)
target_link_libraries(test_policy ${Boost_LIBRARIES})
ADD_TEST(Policy test_policy)

ADD_EXECUTABLE(test_lru tests/test_lru.cpp)
target_link_libraries(test_lru ${Boost_LIBRARIES})
ADD_TEST(LRU test_lru)

ADD_EXECUTABLE(test_mru tests/test_mru.cpp)
target_link_libraries(test_mru ${Boost_LIBRARIES})
ADD_TEST(MRU test_mru)

ADD_EXECUTABLE(test_lfu tests/test_lfu.cpp)
target_link_libraries(test_lfu ${Boost_LIBRARIES})
ADD_TEST(LFU test_lfu)

ADD_EXECUTABLE(test_lfustar tests/test_lfustar.cpp)
target_link_libraries(test_lfustar ${Boost_LIBRARIES})
ADD_TEST(LFU* test_lfustar)

ADD_EXECUTABLE(test_lfuaging tests/test_lfuaging.cpp)
target_link_libraries(test_lfuaging ${Boost_LIBRARIES})
ADD_TEST(LFU-Aging test_lfuaging)

ADD_EXECUTABLE(test_lfuagingstar tests/test_lfuagingstar.cpp)
target_link_libraries(test_lfuagingstar ${Boost_LIBRARIES})
ADD_TEST(LFU*-Aging test_lfuagingstar)

ADD_EXECUTABLE(test_adaptive tests/test_adaptive.cpp)
target_link_libraries(test_adaptive ${Boost_LIBRARIES})
ADD_TEST(ARP test_adaptive)

ADD_EXECUTABLE(test_copy tests/test_copy.cpp)
target_link_libraries(test_copy ${Boost_LIBRARIES})
ADD_TEST(Copy test_copy)

endif(Boost_FOUND)
