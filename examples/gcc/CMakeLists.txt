cmake_minimum_required(VERSION 2.6)
project(GJ_API_example)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O2")
    set(CMAKE_EXE_LINKER_FLAGS "-s")
    add_definitions(-Wall -std=c++0x)
endif()

add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../source"
    "${CMAKE_CURRENT_SOURCE_DIR}/ref"
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../source"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/linux/include"
)

add_executable(
    GJ_API_example
    "../main.cpp"
)

target_link_libraries(
    GJ_API_example 
    GJ_API_lib
)