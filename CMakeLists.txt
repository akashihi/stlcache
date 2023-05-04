#Preamble
cmake_minimum_required (VERSION 3.15)

# Detect if we are bundled and skip building of tests in that case
IF(NOT DEFINED PROJECT_NAME)
	SET(NOT_SUBPROJECT ON)
ENDIF()

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

IF(NOT_SUBPROJECT)
	# Set standard
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)

	# Retrieve the Catch2
	Include(FetchContent)
	FetchContent_Declare(
			Catch2
			GIT_REPOSITORY https://github.com/catchorg/Catch2.git
			GIT_TAG        v3.3.2 # or a later release
	)
	FetchContent_MakeAvailable(Catch2)

	# Unit tests binary
	add_executable(tests
			tests/test_main.cpp tests/test_map.cpp tests/test_map_interface.cpp tests/test_iterator.cpp
			tests/test_victim.cpp tests/test_policy.cpp
			tests/test_lru.cpp tests/test_mru.cpp
			tests/test_lfu.cpp tests/test_lfustar.cpp tests/test_lfuaging.cpp tests/test_lfuagingstar.cpp tests/test_lfu_multi.cpp
			tests/test_adaptive.cpp)
	target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

	# Add tests auto discovery
	list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
	include(CTest)
	include(Catch)
	enable_testing()
	catch_discover_tests(tests)
ENDIF()

if(Boost_FOUND)
enable_testing()
SET(CTEST_ENVIRONMENT "BOOST_TEST_LOG_LEVEL=message")

ADD_EXECUTABLE(test_boost_optional tests/test_boost_optional.cpp)
target_link_libraries(test_boost_optional ${LIBS})
ADD_TEST(boost::optional test_boost_optional)

ADD_EXECUTABLE(test_insert_assign tests/test_insert_assign.cpp)
target_link_libraries(test_insert_assign ${Boost_LIBRARIES})
ADD_TEST(IA test_insert_assign)

#ADD_EXECUTABLE(test_insert_perf tests/test_insert_perf.cpp)
#target_link_libraries(test_insert_perf ${LIBS})

#ADD_EXECUTABLE(test_insdel_perf tests/test_insdel_perf.cpp)
#target_link_libraries(test_insdel_perf ${LIBS})

#ADD_EXECUTABLE(test_victim_perf tests/test_victim_perf.cpp)
#target_link_libraries(test_victim_perf ${LIBS})

endif(Boost_FOUND)
