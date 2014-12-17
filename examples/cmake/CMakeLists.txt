cmake_minimum_required(VERSION 2.6)
project(GJ_API_example)


set(CMAKE_C_COMPILER       "gcc")
set(CMAKE_CXX_COMPILER     "g++")
set(CMAKE_CXX_FLAGS        "-O2")
set(CMAKE_EXE_LINKER_FLAGS "-s")
add_definitions(-Wall -std=c++11)


include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../source"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/include"
)


add_executable(
    GJ_API_example
    "../main.cpp"
    "../../source/gjAPI.cpp"
    "../../source/gjNetwork.cpp"
    "../../source/gjUser.cpp"
    "../../source/gjTrophy.cpp"
    "../../source/gjScore.cpp"
    "../../source/gjDataItem.cpp"
    "../../source/MD5.cpp"
    "../../source/Base64.cpp"
)


if(WIN32)
    target_link_libraries(
      GJ_API_example
      "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/windows/x86/libcurl.a"
    )
else()
    target_link_libraries(
      GJ_API_example
      "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/linux/x64/libcurl.so"
    )
endif()