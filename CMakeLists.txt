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
