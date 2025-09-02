#pragma once

#include <memory>
#include <vector>

#include <btBulletDynamicsCommon.h>

class GameObject {
private:
    btTransform origin;
protected:
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

    // Teleport this object to a transform and reset dynamics
    void teleportTo(const btTransform& t) {
        motion_state->setWorldTransform(t);
        body->setCenterOfMassTransform(t);
        body->setLinearVelocity(btVector3(0, 0, 0));
        body->setAngularVelocity(btVector3(0, 0, 0));
        body->clearForces();
        body->activate(true);
        this->origin = t;
    }
};

// Ground inherits from GameObject
class Ground : public GameObject {
private:
    btStaticPlaneShape shape;
public:
    Ground();
    void draw() const override;
};

// Sphere inherits from GameObject
class Sphere : public GameObject {
private:
    btSphereShape shape;
public:
    Sphere();
    void draw() const override;
};

// Cube inherits from GameObject
class Cube : public GameObject {
private:
    btBoxShape shape;
public:
    Cube();
    void draw() const override;
};