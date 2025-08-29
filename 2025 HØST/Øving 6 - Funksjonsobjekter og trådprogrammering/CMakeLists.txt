cmake_minimum_required(VERSION 3.10)

project(oeving6)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -pthread -Wall -Wextra")
endif()

# Oppgave 1
add_executable(oppgave1 oppgave1.cpp src/chessboard.cpp)

# Oppgave 2
find_package(Boost 1.54.0 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(oppgave2 oppgave2.cpp)

if (WIN32)
    target_link_libraries(oppgave2 ${Boost_LIBRARIES} ws2_32 mswsock)
else()
    target_link_libraries(oppgave2 ${Boost_LIBRARIES})
endif()
