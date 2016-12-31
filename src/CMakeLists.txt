cmake_minimum_required (VERSION 2.6)

include_directories(${AStar_SOURCE_DIR}/include)

add_executable("a-star"
  Main.cpp
  State.cpp
  State2V.cpp
  )

