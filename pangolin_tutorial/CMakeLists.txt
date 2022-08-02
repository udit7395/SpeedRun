cmake_minimum_required(VERSION 3.11)
project(pango_tutorial)

add_definitions("--std=c++17")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Pangolin REQUIRED)
include_directories(${Pangolin_INCLUDE_DIRS})
add_executable(plotTrajectories src/plotTrajectories.cpp)
target_link_libraries(plotTrajectories PRIVATE ${Pangolin_LIBRARIES})