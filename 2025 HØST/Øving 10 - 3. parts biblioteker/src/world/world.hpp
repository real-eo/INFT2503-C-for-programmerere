#pragma once

#include "gameObjects/gameObjects.hpp"

// #include <vector>                                                                    // Included in "gameObjects/gameObjects.hpp"
#include <functional>

// #include <btBulletDynamicsCommon.h>                                                  // Included in "gameObjects/gameObjects.hpp"


// Class for the physics world. Rendering is handled separately by Renderer class.
class World {
private:
    btDefaultCollisionConfiguration collision_configuration;
    btCollisionDispatcher dispatcher;
    btDbvtBroadphase broadphase;
    btSequentialImpulseConstraintSolver solver;

    void freeze();
    void unfreeze();
    
public:
    btDiscreteDynamicsWorld dynamics;

    Ground ground;
    Sphere ground_sphere, falling_sphere;
    std::vector<Cube> cubes;

    std::vector<GameObject*> entityList;

    World();

    void reset();
    void dropBall(float x, float y, float z);                                           // TODO: Update this function to only reposition the ball
    void dropBall();                                                                    
    
    std::function<void()> draw;
};
