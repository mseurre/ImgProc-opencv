cmake_minimum_required(VERSION 3.0)

project(ImgProc++opencv)

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message("------------------------------------")
find_package(OpenCV 3.4 REQUIRED)
message(STATUS "Found OpenCV-Version ${OpenCV_VERSION}")
message(STATUS "OpenCV Include: ${OpenCV_INCLUDE_DIRS}")
message("------------------------------------")
find_package(Boost 1.55 REQUIRED COMPONENTS unit_test_framework)
message(STATUS "Boost Include: ${Boost_INCLUDE_DIR}")
include_directories(${Boost_INCLUDE_DIR})
message("------------------------------------")

include_directories(headers)
include_directories(src/Impl/headers)

add_subdirectory(src/Impl)

SET(HEADERS
        headers/enum.hpp
        headers/Processing.hpp
        headers/Display.hpp
	)

include_directories(headers)

set(SOURCES
        src/Processing.cpp
        src/Display.cpp
	)

set(SAMPLES
        #samples/histogramSample.cpp
        samples/colorConversionSample.cpp
        )

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${SAMPLES})
target_link_libraries(${PROJECT_NAME} impl ${OpenCV_LIBS})

#Boost unit tests
enable_testing()

#equivalent to putting #define BOOST_TEST_DYN_LINK & #define BOOST_TEST_MODULE unit_tests in a file
add_definitions(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MODULE=unit_tests)

add_executable(unit_tests test/tests.cpp)
target_link_libraries(unit_tests impl ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_test(NAME UnitTests COMMAND unit_tests)

add_custom_command(TARGET unit_tests
                   COMMENT "Run unit tests"
                   POST_BUILD COMMAND ctest ARGS --output-on-failure
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
