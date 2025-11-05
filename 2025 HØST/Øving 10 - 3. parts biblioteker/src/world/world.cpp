#include "world/world.hpp"
// #include "gameObjects/gameObjects.hpp"                                       # Included in world/world.hpp


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
    // Freeze the world
    freeze();

    // Reset all entities
    for (auto& entity : entityList) {
        entity->reset();
    }
}

// TODO: Rename and redo this function to only reposition the ball, not drop it
void World::dropBall(float x, float y, float z) {
    btTransform t;

    t.setIdentity();
    t.setOrigin(btVector3(x, y, z));

    falling_sphere.teleport(t);
}

void World::dropBall() {
    // dropBall(0.9f, 3.0f, 0.0f);
    // | TEMPORARY: Ball positioning code. Will be implemented separatly in the function above marked with "TODO"
    btTransform t;

    t.setIdentity();
    // // t.setOrigin(btVector3(x, y, z));
    t.setOrigin(btVector3(0.9f, 3.0f, 0.0f));

    falling_sphere.teleport(t);
    // | ======== |

    // Unfreeze the world
    unfreeze();

}

// Makes all dynamic objects kinematic, essentially freezing the world
void World::freeze() {
    // TODO: Improvement can be done here by storing each entry in the entity list as a pair 
    //     : of <GameObject*, bool>, where the boolean dictates wether or not the object should
    //     : be treated as dynamic, and subsequently made kinematic when freezing the world

    for (auto& entity : entityList) {
        entity->kinematic();
    }
}

// Makes all "frozen" dynamic objects dynamic again, essentially unfreezing the world 
void World::unfreeze() {
    // TODO: Same todo as stated in `World::freeze()`

    for (auto& entity : entityList) {
        entity->dynamic();
    }
}

