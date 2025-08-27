cmake_minimum_required(VERSION 3.10)

project(oeving7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra")

add_executable(oppgave1a oppgave1a.cpp src/fraction.cpp)

add_executable(oppgave2 oppgave2.cpp)
