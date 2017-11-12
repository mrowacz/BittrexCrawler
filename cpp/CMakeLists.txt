cmake_minimum_required(VERSION 3.3.1)

project(BittrexCrawler)

add_subdirectory(cpr)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS_DEBUG "-O2 -g")
SET(CMAKE_C_FLAGS_DEBUG "-O2")
FILE(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(BittrexCrawler ${SOURCE_FILES})
include_directories(
        ${CPR_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/json/src
)
target_link_libraries(BittrexCrawler ${CPR_LIBRARIES})
