#include <gtkmm.h>
#include <iostream>


class Window : public Gtk::Window {
private:
    // * UI 
    Gtk::Box box;

    Gtk::Label firstNameLabel;
    Gtk::Entry firstNameEntry;

    Gtk::Label lastNameLabel;
    Gtk::Entry lastNameEntry;
    
    Gtk::Button combineButton;
    Gtk::Label displayLabel;

    // * Variables
    std::string fullName;

    // * Methods
    void update_button_state() {
        if (!firstNameEntry.get_text().empty() && !lastNameEntry.get_text().empty()) {
            combineButton.set_sensitive(true);
            return;   
        }

        combineButton.set_sensitive(false);
    }

public:
    Window() : box(Gtk::Orientation::ORIENTATION_VERTICAL) {
        // * Configuration
        // Configure the window
        this->set_title("Øving 4");                 // Merk: Dette kan også gjøres i main funksjonen via. `window.set_title(std::string)`

        // Configure the lables
        firstNameLabel.set_label("First name: ");
        lastNameLabel.set_label("Last name: ");

        // Configure the button
        combineButton.set_label("Combine names");
        update_button_state();

        // * UI
        // Pack elements into bounding box
        box.pack_start(firstNameLabel);
        box.pack_start(firstNameEntry);

        box.pack_start(lastNameLabel);
        box.pack_start(lastNameEntry); 

        box.pack_start(combineButton); 
        box.pack_start(displayLabel);

        // Add bounding box to screen
        add(box);   
        show_all(); 

        // * Handling
        firstNameEntry.signal_changed().connect([this]() { update_button_state(); });
        lastNameEntry.signal_changed().connect([this]() { update_button_state(); });

        combineButton.signal_clicked().connect([this]() {
            fullName = firstNameEntry.get_text() + " " + lastNameEntry.get_text();
            displayLabel.set_text("Names combined: " + fullName);
        });
    }
};

int main() {
    auto app = Gtk::Application::create();
    
    Window window;

    return app->run(window);
}
