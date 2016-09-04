cmake_minimum_required(VERSION 3.5)
project(socketbadge)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")

set(SOURCE_FILES socketbadge.c)

add_executable(socketbadge ${SOURCE_FILES})
target_link_libraries(socketbadge usb-1.0 pthread)