#pragma once

// #include <SFML/Graphics.hpp>
// #include <SFML/OpenGL.hpp>
// #include <SFML/System.hpp>

#include <glm/glm.hpp>
#include <chrono>


// Forward declaration
class World;

// Class for rendering the world
class Renderer {
private: 
    World& world;
public:
    Renderer(World& world);
    static void setup_opengl();
    void render_frame(const glm::vec3& camera, std::chrono::system_clock::time_point& last_time);
};
