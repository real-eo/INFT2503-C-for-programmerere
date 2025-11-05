#pragma once

#include <memory>
#include <vector>
#include <array>

#include <btBulletDynamicsCommon.h>


class GameObject {
private:
    btTransform origin;
    btTransform position = btTransform::getIdentity();
    // TODO: Figure out if the reference to m_origin in getPositionReferences() is immutable during the 
    //     : lifetime of the GameObject instance. If so, store the reference directly as a member variable
    
protected:
    std::unique_ptr<btDefaultMotionState> motion_state;

public:
    std::unique_ptr<btRigidBody> body;

    // * Constructors & Deconstructor
    virtual ~GameObject() = default;
    virtual void draw() const = 0;
    
    // * Getters
    btTransform getOrigin() const { return this->origin; }
    std::array<float*, 4U> getPositionPointers();
    
    // * Setters
    void setOrigin(const btVector3& position) {
        this->origin.setIdentity();
        this->origin.setOrigin(position);
    }
    
    // * Inlines
    inline void updatePosition() { motion_state->setWorldTransform(origin * position); }
    
    // * Functions
    void reset();
    void kinematic();
    void dynamic();
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
