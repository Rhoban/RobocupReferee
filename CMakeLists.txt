cmake_minimum_required(VERSION 3.16.3)
project(robocup_referee)

## Building the main
option (BUILD_REFEREE_BIN "Build referee binary" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra")

set(SOURCES
  src/robocup_referee/constants.cpp
  src/robocup_referee/game_state.cpp
  src/robocup_referee/referee_client.cpp
  src/robocup_referee/robot.cpp
  src/robocup_referee/team.cpp
  src/robocup_referee/time_stamp.cpp
  src/robocup_referee/utilities.cpp
)

add_library(robocup_referee ${SOURCES})
target_link_libraries (robocup_referee PUBLIC rhoban_utils hl_monitoring)
target_include_directories(robocup_referee PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if (BUILD_REFEREE_BIN)
    add_executable (referee src/main.cpp)
    target_link_libraries (referee robocup_referee pthread)

    add_executable (dump_referees src/dump_referees.cpp)
    target_link_libraries (dump_referees robocup_referee pthread)
endif ()
