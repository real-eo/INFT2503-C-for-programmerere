#include "rendering/world.hpp"
#include "world/world.hpp"
// #include "gameObjects/gameObjects.hpp"                                       # Included in world/world.hpp 

// Open GL
#include <SFML/OpenGL.hpp>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif


Renderer::Renderer(World& world) : world(world) {
    // Only construct the class with a corresponding World instance
    world.draw = [&]() {
        for (const auto& entity : world.entityList) {
            entity->draw();
        }
    };
}

void Renderer::setup_opengl() {
    // Various settings
    glClearColor(0.5, 0.5, 0.5, 0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[] = {-3.0, 4.0, 0.0, 1.0};
    GLfloat spot_direction[] = {1.0, -1.0, -0.5};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
    glEnable(GL_COLOR_MATERIAL);

    // Setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    gluPerspective(45.0, 4.0 / 3.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);

    // ImGui settings
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Renderer::render_frame(const glm::vec3& camera, std::chrono::system_clock::time_point& last_time) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(camera.x, camera.y, camera.z,                 // Camera position in World Space
                        camera.x, camera.y, 0.0,            // Camera looks towards this position
                        0.0, 1.0, 0.0);                     // Up

    auto time = std::chrono::system_clock::now();
    world.dynamics.stepSimulation(std::chrono::duration<float>(time - last_time).count());
    last_time = time;

    world.draw();
}
