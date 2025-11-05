#include "rendering/gui.hpp"
#include "world/world.hpp"


Gui::Gui(World& world, sf::RenderWindow& window) : world(world), window(window) {
    // Initialize ImGui-SFML
    if (!ImGui::SFML::Init(window)) {
        throw std::runtime_error("Failed to initialize ImGui::SFML");
    }

    // Position the GUI window
    positionGui();
}


Gui::~Gui() {
    ImGui::SFML::Shutdown();                                                            // Handle shutdown here to follow RAII principles 
}


// |----   PRIVATE   ----| 
void Gui::positionGui() {
    // Start a new ImGui frame so SetNextWindowPos
    // and SetNextWindowSize gets queued correctly
    ImGui::NewFrame();

    // Position the GUI window
    ImGui::SetNextWindowPos(
        ImVec2(0, ImGui::GetIO().DisplaySize.y - 158),                                  // Position     (158 pixels is the height of the GUI window)                    
        ImGuiCond_Once                                                                  // Condition    (Only set once)
    );
    
    ImGui::SetNextWindowSize(ImVec2(window.getSize().x, 158));                          // ? 158 pixels is the height of the GUI window

    // Ensure the GUI is created and built in order
    // for the position- and sizing to take effect
    ImGui::Begin("Controls");                                                           // ? No flags for restricting the GUI's position- and sizing is needed   
    ImGui::End();                                                                       // ? here, as the frame gets preemptively ended before rendering begins 

    // Preemptively end the frame to avoid rendering 
    // the unrestricted window, so it's certain that
    // the ImGui's position- and sizing take effect
    ImGui::EndFrame();
}


void Gui::handle() {
    ImGui::Begin("Controls", nullptr, 
    //  [Flags] ----------------------------+
        ImGuiWindowFlags_NoMove             | 
        ImGuiWindowFlags_NoResize           |
        ImGuiWindowFlags_NoCollapse         
    );


    // * Control buttons
    ImVec2 full { ImGui::GetContentRegionAvail().x, 0.0f };

    if (ImGui::Button("Restart game", full)) {                                          // Restart game button
        world.reset();
    }
    
    if (ImGui::Button("Drop ball", full)) {                                             // Drop ball button
        world.dropBall();
    }


    // * Position sliders
    std::array<float*, 4U> position_ptr = world.falling_sphere.getPositionPointers();

    // Set the width for both sliders
    ImGui::PushItemWidth(-FLT_MIN);


    // Horizontal position slider
    btScalar* horizontalPosition = position_ptr[0];

    ImGui::TextUnformatted("Horizontal ball position");
    if (ImGui::SliderFloat("##hpos", horizontalPosition, -4.25f, 2.5f)) {
        world.falling_sphere.updatePosition();
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {                                 // Reset slider value on right-click
        *horizontalPosition = 0.0f;
        world.falling_sphere.updatePosition();
    }


    // Vertical position slider
    btScalar* verticalPosition = position_ptr[1];
    
    ImGui::TextUnformatted("Vertical ball position");
    if (ImGui::SliderFloat("##vpos", verticalPosition, -3.0f, 0.5f)) {
        world.falling_sphere.updatePosition();
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {                                 // Reset slider value on right-click
        *verticalPosition = 0.0f;
        world.falling_sphere.updatePosition();
    }


    // Pop the item width after both sliders have been created
    ImGui::PopItemWidth();


    ImGui::End();
}
