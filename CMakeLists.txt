#Preamble
cmake_minimum_required (VERSION 2.6)
project (stlcache)

#Environment detection
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#Check for libraries presence
SET(BOOST_ROOT "D:\\Boost")
SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost 1.42.0 COMPONENTS system unit_test_framework)

#Build configuration
include_directories(${CMAKE_BINARY_DIR}
		    ${CMAKE_SOURCE_DIR}/src
		    ${Boost_INCLUDE_DIRS}
)

if(Boost_FOUND)
enable_testing()
SET(CTEST_ENVIRONMENT "BOOST_TEST_LOG_LEVEL=message")

ADD_EXECUTABLE(test_map src/test_map.cpp)
target_link_libraries(test_map ${Boost_LIBRARIES})
ADD_TEST(Map test_map)

ADD_EXECUTABLE(test_victim src/test_victim.cpp)
target_link_libraries(test_victim ${Boost_LIBRARIES})
ADD_TEST(Victim test_victim)

ADD_EXECUTABLE(test_policy src/test_policy.cpp)
target_link_libraries(test_policy ${Boost_LIBRARIES})
ADD_TEST(Policy test_policy)

ADD_EXECUTABLE(test_lru src/test_lru.cpp)
target_link_libraries(test_lru ${Boost_LIBRARIES})
ADD_TEST(LRU test_lru)

ADD_EXECUTABLE(test_mru src/test_mru.cpp)
target_link_libraries(test_mru ${Boost_LIBRARIES})
ADD_TEST(MRU test_mru)

ADD_EXECUTABLE(test_lfu src/test_lfu.cpp)
target_link_libraries(test_lfu ${Boost_LIBRARIES})
ADD_TEST(LFU test_lfu)

ADD_EXECUTABLE(test_lfustar src/test_lfustar.cpp)
target_link_libraries(test_lfustar ${Boost_LIBRARIES})
ADD_TEST(LFU* test_lfustar)

ADD_EXECUTABLE(test_lfuaging src/test_lfuaging.cpp)
target_link_libraries(test_lfuaging ${Boost_LIBRARIES})
ADD_TEST(LFU-Aging test_lfuaging)

ADD_EXECUTABLE(test_lfuagingstar src/test_lfuagingstar.cpp)
target_link_libraries(test_lfuagingstar ${Boost_LIBRARIES})
ADD_TEST(LFU*-Aging test_lfuagingstar)

ADD_EXECUTABLE(test_copy src/test_copy.cpp)
target_link_libraries(test_copy ${Boost_LIBRARIES})
ADD_TEST(Copy test_copy)

endif(Boost_FOUND)
