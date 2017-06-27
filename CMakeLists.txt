cmake_minimum_required(VERSION 2.8)
project(vision_tools)

option(DISPLAY "whether to have a display" OFF)

set(CMAKE_CXX_FLAGS "-std=c++11")
find_package(OpenCV)
if (OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
else()
    message(FATAL_ERROR, "OpenCV is not found")
endif()

set(FRAME_EXTACT_FILES ./src/frameExtract.cpp)

add_executable(frameExtract ${FRAME_EXTACT_FILES})
target_link_libraries(frameExtract ${OpenCV_LIBS})
