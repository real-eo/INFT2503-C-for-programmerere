#pragma once

#include "gameObjects/gameObjects.hpp"

// #include <vector>                                                // Included in "gameObjects/gameObjects.hpp"
#include <functional>

// #include <btBulletDynamicsCommon.h>                              // Included in "gameObjects/gameObjects.hpp"


// // Forward declaration
// // class GameObject;

// Class for the physics world. Rendering is handled separately by Renderer class.
class World {
private:
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

    World();

    void reset();
    void dropBall(float x, float y, float z);
    void dropBall();

    std::function<void()> draw;
};
