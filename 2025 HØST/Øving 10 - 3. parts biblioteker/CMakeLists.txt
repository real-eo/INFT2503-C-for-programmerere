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

include_directories(imgui imgui-sfml src)

# Game objects library (also compiles the GLUT solid shapes C source)
add_library(game_objects
  src/game_objects/GameObjects.cpp
  glut_shapes.c
)
target_link_libraries(game_objects PUBLIC ${OPENGL_LIBRARIES} ${BULLET_LIBRARIES})
target_include_directories(game_objects PUBLIC src)

# World library depends on game_objects
add_library(world_lib
  src/world/World.cpp
)
target_link_libraries(world_lib PUBLIC game_objects ${BULLET_LIBRARIES})
target_include_directories(world_lib PUBLIC src)

# Main executable (application stays in main.cpp)
add_executable(oppgave
  main.cpp
  imgui/imgui.cpp
  imgui/imgui_widgets.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_tables.cpp
  imgui-sfml/imgui-SFML.cpp
)

target_link_libraries(oppgave
  ${CMAKE_THREAD_LIBS_INIT}
  SFML::System SFML::Window SFML::Graphics
  ${OPENGL_LIBRARIES}
  ${BULLET_LIBRARIES}
  world_lib
  game_objects
)