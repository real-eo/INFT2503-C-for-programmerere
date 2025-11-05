#include "rendering/gui.hpp"
#include "world/world.hpp"


Gui::Gui(World& world, sf::RenderWindow& window) : world(world), window(window) {
    if (!ImGui::SFML::Init(window)){
        throw "Failed to initialize Imgui::SFML";
    }
}

Gui::~Gui() {
    ImGui::SFML::Shutdown();                                                            // Handle shutdown here to follow RAII principles 
}


void Gui::handle() {
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
}