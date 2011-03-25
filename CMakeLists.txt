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
#FILE(GLOB LOCAL_INCLUDES ${CMAKE_SOURCE_DIR}/src/*)
include_directories(${CMAKE_BINARY_DIR}
#		    ${LOCAL_INCLUDES}
		    ${Boost_INCLUDE_DIRS}
)

if(Boost_FOUND)
enable_testing()
SET(CTEST_ENVIRONMENT "BOOST_TEST_LOG_LEVEL=message")

ADD_EXECUTABLE(test_map src/test_map.cpp)
target_link_libraries(test_map ${Boost_LIBRARIES})
ADD_TEST(Map test_map)

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

endif(Boost_FOUND)
