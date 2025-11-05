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

    ImGui::Begin("Controls", nullptr, 
    //  [Flags] ----------------------------+
        ImGuiWindowFlags_NoMove             | 
        ImGuiWindowFlags_NoResize           |
        ImGuiWindowFlags_NoCollapse         |
        ImGuiWindowFlags_AlwaysAutoResize   
    );

    // * Control buttons
    if (ImGui::Button("Restart game")) {
        world.reset();
    }

    if (ImGui::Button("Drop ball")) {
        world.dropBall();
    }

    // * Position sliders
    std::array<float*, 4U> position_ptr = world.falling_sphere.getPositionPointers();


    // Horizontal position slider
    btScalar* horizontalPosition = position_ptr[0];

    if (ImGui::SliderFloat("Horizontal ball position", horizontalPosition, -4.25, 2.5)) {
        world.falling_sphere.updatePosition();
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {                                 // Reset slider value on right-click
        *horizontalPosition = 0.0f;
        world.falling_sphere.updatePosition();
    }


    // Vertical position slider
    btScalar* verticalPosition = position_ptr[1];
    
    if (ImGui::SliderFloat("Vertical ball position", verticalPosition, -3.0, 0.5)) {
        world.falling_sphere.updatePosition();
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {                                 // Reset slider value on right-click
        *verticalPosition = 0.0f;
        world.falling_sphere.updatePosition();
    }

    ImGui::End();
}