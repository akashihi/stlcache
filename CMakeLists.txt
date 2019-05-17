#Preamble
cmake_minimum_required (VERSION 3.5)
project (STL::Cache)

#Environment detection
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#Check for libraries presence
SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost 1.58.0 COMPONENTS system unit_test_framework)

#Add MT libraries
SET(LIBS ${Boost_LIBRARIES})

#Documentation stuff
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include(UseDoxygen)
set(DOXYFILE_LATEX OFF)

#Build configuration
include_directories(${CMAKE_BINARY_DIR}
		    ${CMAKE_SOURCE_DIR}/include
		    ${Boost_INCLUDE_DIRS}
)

install(DIRECTORY include/stlcache DESTINATION include)

if(Boost_FOUND)
enable_testing()
SET(CTEST_ENVIRONMENT "BOOST_TEST_LOG_LEVEL=message")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

ADD_EXECUTABLE(test_map tests/test_map.cpp)
target_link_libraries(test_map ${LIBS})
ADD_TEST(Map test_map)

ADD_EXECUTABLE(test_victim tests/test_victim.cpp)
target_link_libraries(test_victim ${LIBS})
ADD_TEST(Victim test_victim)

ADD_EXECUTABLE(test_policy tests/test_policy.cpp)
target_link_libraries(test_policy ${LIBS})
ADD_TEST(Policy test_policy)

ADD_EXECUTABLE(test_lru tests/test_lru.cpp)
target_link_libraries(test_lru ${LIBS})
ADD_TEST(LRU test_lru)

ADD_EXECUTABLE(test_mru tests/test_mru.cpp)
target_link_libraries(test_mru ${LIBS})
ADD_TEST(MRU test_mru)

ADD_EXECUTABLE(test_lfu tests/test_lfu.cpp)
target_link_libraries(test_lfu ${LIBS})
ADD_TEST(LFU test_lfu)

ADD_EXECUTABLE(test_lfustar tests/test_lfustar.cpp)
target_link_libraries(test_lfustar ${LIBS})
ADD_TEST(LFU* test_lfustar)

ADD_EXECUTABLE(test_lfuaging tests/test_lfuaging.cpp)
target_link_libraries(test_lfuaging ${LIBS})
ADD_TEST(LFU-Aging test_lfuaging)

ADD_EXECUTABLE(test_lfuagingstar tests/test_lfuagingstar.cpp)
target_link_libraries(test_lfuagingstar ${LIBS})
ADD_TEST(LFU*-Aging test_lfuagingstar)

ADD_EXECUTABLE(test_adaptive tests/test_adaptive.cpp)
target_link_libraries(test_adaptive ${LIBS})
ADD_TEST(ARC test_adaptive)

ADD_EXECUTABLE(test_boost_optional tests/test_boost_optional.cpp)
target_link_libraries(test_boost_optional ${LIBS})
ADD_TEST(boost::optional test_boost_optional)

ADD_EXECUTABLE(test_insert_assign tests/test_insert_assign.cpp)
target_link_libraries(test_insert_assign ${Boost_LIBRARIES})
ADD_TEST(IA test_insert_assign)

ADD_EXECUTABLE(test_insert_perf tests/test_insert_perf.cpp)
target_link_libraries(test_insert_perf ${LIBS})

ADD_EXECUTABLE(test_insdel_perf tests/test_insdel_perf.cpp)
target_link_libraries(test_insdel_perf ${LIBS})

ADD_EXECUTABLE(test_victim_perf tests/test_victim_perf.cpp)
target_link_libraries(test_victim_perf ${LIBS})

ADD_EXECUTABLE(test_lfu_multi tests/test_lfu_multi.cpp)
target_link_libraries(test_lfu_multi ${LIBS})
ADD_TEST(LFU-MultiIndex test_lfu_multi)

ADD_EXECUTABLE(test_map_interface tests/test_map_interface.cpp)
target_link_libraries(test_map_interface ${LIBS})
ADD_TEST(MapInterface test_map_interface)

endif(Boost_FOUND)
