#pragma once

// SFML
#include <SFML/Graphics.hpp>                                                // Also includes <SFML/Window.hpp>
#include <SFML/System.hpp>                                                                                        

// ImGui
#include "imgui-SFML.h"
#include "imgui.h"


// Forward declaration
class World;

// Class for rendering the world
class Gui {
private: 
    World& world;
    sf::RenderWindow& window;

public:
    // Constructors & Deconstructor
    Gui(World& world, sf::RenderWindow& window);
    ~Gui();

    // Inlines
    inline void process(const sf::Event& event) {   
        ImGui::SFML::ProcessEvent(window, event); 
    }

    inline void update(const sf::Time& dt) {               
        ImGui::SFML::Update(window, dt); 
    }

    inline void render() {
        window.pushGLStates();
        ImGui::SFML::Render(window);
        window.popGLStates();
    }

    // Functions
    void handle();

};
