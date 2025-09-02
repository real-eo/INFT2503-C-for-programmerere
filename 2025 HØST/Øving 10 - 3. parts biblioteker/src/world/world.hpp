#pragma once

#include <vector>
#include <functional>

#include <btBulletDynamicsCommon.h>

#include "game_objects/GameObjects.hpp"

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
