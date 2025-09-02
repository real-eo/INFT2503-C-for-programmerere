





// #include <chrono>                                   



// #include <glm/glm.hpp>

// #include <btBulletDynamicsCommon.h>





// SFML
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>

// ImGui
#include "imgui-SFML.h"
#include "imgui.h"

// Source
// #include "src/gameObjects/gameObjects.hpp"                                           // Not needed here
#include "src/world/world.hpp"
#include "src/rendering/world.hpp"
#include "src/rendering/gui.hpp"

// main.cpp
#include <iostream>


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
