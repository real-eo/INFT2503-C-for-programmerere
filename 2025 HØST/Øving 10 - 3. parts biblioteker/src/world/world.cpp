#include "world/World.hpp"

World::World()
: dispatcher(&collision_configuration),
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

void World::reset() {
    // Reset velocities
    for (auto& entity : entityList) {
        entity->body->setLinearVelocity(btVector3(0, 0, 0));
        entity->body->setAngularVelocity(btVector3(0, 0, 0));
    }
    // Reset positions
    for (auto& entity : entityList) {
        entity->body->setCenterOfMassTransform(entity->getTransform());
    }
}

void World::dropBall(float x, float y, float z) {
    btTransform t;
    t.setIdentity();
    t.setOrigin(btVector3(x, y, z));
    falling_sphere.teleportTo(t);
}

// No-arg overload to keep main.cpp working
void World::dropBall() {
    dropBall(0.9f, 3.0f, 0.0f);
}
