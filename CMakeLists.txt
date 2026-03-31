cmake_minimum_required(VERSION 3.16.3)
project(robocup_referee)

## Building the main
option (BUILD_REFEREE_BIN "Build referee binary" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra")

set(SOURCES
  #src/robocup_referee/constants.cpp
  #src/robocup_referee/game_state.cpp
  src/robocup_referee/referee_client.cpp
  #src/robocup_referee/robot.cpp
  #src/robocup_referee/team.cpp
  src/robocup_referee/time_stamp.cpp
  src/robocup_referee/utilities.cpp
  src/robocup_referee/gc_msg.cpp
)

add_library(robocup_referee SHARED ${SOURCES})
target_link_libraries (robocup_referee PUBLIC rhoban_utils)
target_include_directories(robocup_referee PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if (BUILD_REFEREE_BIN)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tools)
    add_executable (gc_referee src/gc_referee.cpp)
    target_link_libraries (gc_referee robocup_referee pthread)

    add_executable (gc_dump_msg src/gc_dump_msg.cpp)
    target_link_libraries (gc_dump_msg robocup_referee pthread)

    add_executable (gc_raw_broadcast src/gc_raw_broadcast.cpp)
    target_link_libraries (gc_raw_broadcast robocup_referee pthread)
endif ()
