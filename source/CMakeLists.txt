if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O2")
    set(CMAKE_EXE_LINKER_FLAGS "-s")
    add_definitions(-Wall -std=c++0x)
endif()

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/../libraries/linux/include"
)

set(
    GJ_SRC
    gjAPI.cpp
    gjNetwork.cpp
    gjUser.cpp
    gjTrophy.cpp
    gjScore.cpp
    gjDataItem.cpp
    MD5.cpp
    Base64.cpp
)

add_library(
    GJ_API_lib 
    ${GJ_SRC}
)

target_link_libraries(
    GJ_API_lib
    "${CMAKE_CURRENT_SOURCE_DIR}/../libraries/linux/libcurl.so"
)