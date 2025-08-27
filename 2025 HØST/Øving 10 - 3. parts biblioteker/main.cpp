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

#include <btBulletDynamicsCommon.h>

#include <chrono>

#include <iostream>
#include <functional>

extern "C" {
// Implementation is in glut_shapes.c
void glutSolidCube(GLdouble size);
}

// Small fix due to M_PI being omitted when compiling using strict standard flags on Windows MinGW and Windows MSYS2
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


class GameObject {
private:
    btTransform origin;
protected:
    // std::unique_ptr<btCollisionShape> shape;
    std::unique_ptr<btDefaultMotionState> motion_state;
public:
    std::unique_ptr<btRigidBody> body;

    virtual ~GameObject() = default;
    virtual void draw() const = 0;

    void setOrigin(const btVector3& position) {
        this->origin.setIdentity();
        this->origin.setOrigin(position);
    }

    btTransform getTransform() const { return this->origin; }
};

// Ground inherits from GameObject
class Ground : public GameObject {
private:
    btStaticPlaneShape shape;
public:
    Ground() : shape(btVector3(0.0, 1.0, 0.0), 0.0) {
        motion_state = std::make_unique<btDefaultMotionState>();

        body = std::make_unique<btRigidBody>(0.0, motion_state.get(), &shape);

        body->setRollingFriction(0.1);
        body->setRestitution(0.8);
    }

    void draw() const override {
        auto position = body->getCenterOfMassPosition();
        glBegin(GL_TRIANGLE_STRIP);
        glColor3f(0.0, 1.0, 0.0);
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(position.x() - 5.0, position.y(), position.z() + 5.0);
        glVertex3f(position.x() - 5.0, position.y(), position.z() - 5.0);
        glVertex3f(position.x() + 5.0, position.y(), position.z() + 5.0);
        glVertex3f(position.x() + 5.0, position.y(), position.z() - 5.0);
        glEnd();
    }
};

// Sphere inherits from GameObject
class Sphere : public GameObject {
    btSphereShape shape;
public:
    Sphere() : shape(btScalar(0.1)) {
        motion_state = std::make_unique<btDefaultMotionState>();
        body = std::make_unique<btRigidBody>(1.0, motion_state.get(), &shape);

        auto mass = 1.0 / body->getInvMass();

        btVector3 inertia;
        shape.calculateLocalInertia(mass, inertia);
        body->setMassProps(mass, inertia);
        body->setRollingFriction(0.2);
        body->setRestitution(0.8);
    }

    void draw() const override {
        glPushMatrix();
        auto position = body->getCenterOfMassPosition();
        glTranslatef(position.x(), position.y(), position.z());
        glColor3f(0.0, 0.0, 1.0);
        auto quadric = gluNewQuadric();
        gluSphere(quadric, shape.getRadius(), 32, 32);
        gluDeleteQuadric(quadric);
        glPopMatrix();
    }
};

// Cube inherits from GameObject
class Cube : public GameObject {
    btBoxShape shape;
public:
    Cube() : shape({0.1, 0.1, 0.1}) {
        motion_state = std::make_unique<btDefaultMotionState>();
        body = std::make_unique<btRigidBody>(1.0, motion_state.get(), &shape);
        
        auto mass = 1.0 / body->getInvMass();
        
        btVector3 inertia;
        shape.calculateLocalInertia(mass, inertia);
        body->setMassProps(mass, inertia);
        body->setFriction(0.5);
        body->setRollingFriction(0.1);
        body->setRestitution(0.2);
    }

    void draw() const override {
        glPushMatrix();
        auto position = body->getCenterOfMassPosition();
        glTranslatef(position.x(), position.y(), position.z());
        auto orientation = body->getOrientation();
        auto axis = orientation.getAxis().normalize();
        glRotatef(orientation.getAngle() * 180.0 / M_PI, axis.x(), axis.y(), axis.z());
        glColor3f(1.0, 0.0, 0.0);
        glutSolidCube(0.2);
        glPopMatrix();
    }
};

class World {
    btDefaultCollisionConfiguration collision_configuration;
    btCollisionDispatcher dispatcher;
    btDbvtBroadphase broadphase;
    btSequentialImpulseConstraintSolver solver;

public:
    btDiscreteDynamicsWorld dynamics;

    Ground ground;
    Sphere ground_sphere, falling_sphere;
    std::vector<Cube> cubes;
    
    std::vector<GameObject*> entityList;


    World() : dispatcher(&collision_configuration),
              dynamics(&dispatcher, &broadphase, &solver, &collision_configuration),
              cubes(6) {
        dynamics.setGravity(btVector3(0, -10.0, 0));

        // Add objects to entity list
        entityList.push_back(&ground);
        entityList.push_back(&ground_sphere);
        entityList.push_back(&falling_sphere);
        for (auto &cube : cubes)
            entityList.push_back(&cube);

        // Add objects to the physics engine
        for (auto &entity : entityList) {
            dynamics.addRigidBody(entity->body.get());
        }

    
        // Position ground
        ground.setOrigin(btVector3(0.0, 0.0, 0.0));
        
        // Position spheres
        ground_sphere.setOrigin(btVector3(1.0, 0.1, 0.0));
        falling_sphere.setOrigin(btVector3(0.9, 3.0, 0.0));
        
        // Position cubes
        cubes[0].setOrigin(btVector3(-1.0, 0.1, -0.2));
        cubes[1].setOrigin(btVector3(-1.0, 0.1, 0.0));
        cubes[2].setOrigin(btVector3(-1.0, 0.1, 0.2));
        cubes[3].setOrigin(btVector3(-1.0, 0.3, -0.1));
        cubes[4].setOrigin(btVector3(-1.0, 0.3, 0.1));
        cubes[5].setOrigin(btVector3(-1.0, 0.5, 0.0));

        // Reset the world
        reset();
    }

    void reset() {
        // Reset positions of all game objects
        for (auto& entity : entityList) {
            entity->body->setCenterOfMassTransform(entity->getTransform());
        }
    }

    std::function<void()> draw;
};

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
                // Implementation needed
                std::cout << "DEBUG" << std::endl;
                world.reset();
            }
            if (ImGui::Button("Drop ball")) {
                // Implementation needed
                // world.dropBall();
            }
            float horizontal_position = 0.0f;
            if (ImGui::SliderFloat("Horizontal ball position", &horizontal_position, 0.0, 10.0)) {
                // Implementation needed
                // world.setBallPosition(horizontal_position);
            }
            float vertical_position = 0.0f;
            if (ImGui::VSliderFloat("Vertical ball position", {20, 100}, &vertical_position, 0.0, 10.0)) {
                // Implementation needed
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
};

int main() {
    Application application;
    
    application.start();
}
