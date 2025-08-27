cmake_minimum_required(VERSION 3.10)

project(oeving5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra")

# Oppgave 1
add_executable(oppgave oppgave.cpp)