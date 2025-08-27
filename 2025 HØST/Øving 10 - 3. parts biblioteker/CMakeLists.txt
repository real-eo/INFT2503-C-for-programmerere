cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(oeving10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include \"${PROJECT_SOURCE_DIR}/imgui-sfml/imconfig-SFML.h\" -std=c++17 -Wall -Wextra -Wno-reorder -Wno-ignored-qualifiers")

if(APPLE)
  add_definitions(-DGL_SILENCE_DEPRECATION)
  include_directories(/opt/homebrew/include)
  include_directories(/usr/local/include)
endif()

find_package(SFML 3.0 COMPONENTS System Window Graphics REQUIRED)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIR})

find_package(Bullet REQUIRED)
include_directories(${BULLET_INCLUDE_DIRS})

find_package(Threads REQUIRED)

include_directories(imgui imgui-sfml)

add_executable(oppgave main.cpp glut_shapes.c imgui/imgui.cpp imgui/imgui_widgets.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui-sfml/imgui-SFML.cpp)

target_link_libraries(oppgave ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(oppgave SFML::System SFML::Window SFML::Graphics)
target_link_libraries(oppgave ${OPENGL_LIBRARIES})
target_link_libraries(oppgave ${BULLET_LIBRARIES})
