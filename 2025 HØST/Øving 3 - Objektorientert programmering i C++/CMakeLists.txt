cmake_minimum_required(VERSION 3.10)

project(oeving3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra")

# include_directories(src include)
# file(GLOB SOURCES "*.cpp")

add_executable(oppgave2 oppgave2.cpp)
add_executable(oppgave3 oppgave3.cpp commodity.cpp)
add_executable(oppgave4 oppgave4.cpp)
