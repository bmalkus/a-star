cmake_minimum_required (VERSION 2.6)

include_directories(${AStar_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -Wall")

add_executable("a-star"
  Main.cpp
  AStar.cpp
  AStarHelper.cpp
  State.cpp
  )

