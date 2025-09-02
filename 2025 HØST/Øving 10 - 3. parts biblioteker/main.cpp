#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <glm/glm.hpp>

// #include <btBulletDynamicsCommon.h>

#include <chrono>

#include <iostream>
// #include <functional>

// extern "C" {
// // Implementation is in glut_shapes.c
// void glutSolidCube(GLdouble size);
// }

// // Small fix due to M_PI being omitted when compiling using strict standard flags on Windows MinGW and Windows MSYS2
// #ifndef M_PI
// #define M_PI 3.14159265358979323846
// #endif

#include "world/World.hpp"

// Class for rendering the world
class Renderer {
private:
    World& world;
public:
    Renderer(World& world) : world(world) {
        // Only construct the class with a corresponding World instance
        world.draw = [&]() {
            for (const auto& entity : world.entityList) {
                entity->draw();
            }
        };
    }

    // Should only be called once as it's just for configuring OpenGL
    static void setup_opengl() {
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

    void render_frame(const glm::vec3& camera, std::chrono::system_clock::time_point& last_time) {
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
};



class Application {
    sf::RenderWindow window;
    World world;

    Renderer renderer;


public:
    Application() 
    : window(sf::VideoMode({800, 600}), "SFML Example"),
      world(),
      renderer(world) {
        window.setFramerateLimit(144);
        window.setVerticalSyncEnabled(true);

        // Rendering
        Renderer::setup_opengl();

        if (!ImGui::SFML::Init(window))
            throw "Failed to initialize Imgui::SFML";
    }

    void start() {
        glm::vec3 camera(0.0, 1.0, 6.0);

        sf::Clock delta_clock;

        auto last_time = std::chrono::system_clock::now();
        bool running = true;
        while (running) {
            // Handle events
            while (auto event = window.pollEvent()) {
                ImGui::SFML::ProcessEvent(window, *event);
                if (auto keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                        window.close();
                        running = false;
                    }
                } else if (event->is<sf::Event::Closed>()) {
                    window.close();
                    running = false;
                }
            }

            ImGui::SFML::Update(window, delta_clock.restart());

            ImGui::Begin("ImGui");
            if (ImGui::Button("Restart game")) {
                world.reset();
            }
            if (ImGui::Button("Drop ball")) {
                world.dropBall();
            }
            float horizontal_position = 0.0f;
            if (ImGui::SliderFloat("Horizontal ball position", &horizontal_position, 0.0, 10.0)) {
                // TODO: Implementation needed
                // world.setBallPosition(horizontal_position);
            }
            float vertical_position = 0.0f;
            if (ImGui::VSliderFloat("Vertical ball position", {20, 100}, &vertical_position, 0.0, 10.0)) {
                // TODO: Implementation needed
                // world.setBallPosition(vertical_position);
            }
            ImGui::End();

            // World Rendering
            renderer.render_frame(camera, last_time);

            // ImGUI
            window.pushGLStates();
            ImGui::SFML::Render(window);
            window.popGLStates();

            // Swap buffer (show result)
            window.display();


        }
        ImGui::SFML::Shutdown();
    }

    class Gui;
};

class Application::Gui {
    
};

int main() {
    Application application;
    
    application.start();
}
