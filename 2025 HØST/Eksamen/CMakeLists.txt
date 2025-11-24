# Project CMake configuration for Eksamen
cmake_minimum_required(VERSION 3.10)
project(Eksamen)

# Variables
set(CMAKE_CXX_STANDARD 14)

# Find GTKMM
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

# Executables for each oppgave
# add_executable(oppgave1 oppgave1/oppgave1.cpp)
# add_executable(oppgave2 oppgave2/oppgave2.cpp)
# add_executable(oppgave3 oppgave3/oppgave3.cpp)
# add_executable(oppgave4 oppgave4/oppgave4.cpp)
add_executable(oppgave5 oppgave5/oppgave5.cpp)

# Link GTKMM libraries
target_link_libraries(oppgave5 ${GTKMM_LIBRARIES})