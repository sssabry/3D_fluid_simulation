cmake_minimum_required(VERSION 3.10)

project(FluidSimulation VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(fluid_sim main.cpp FluidCube.cpp FluidSim.cpp)

target_include_directories(fluid_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
