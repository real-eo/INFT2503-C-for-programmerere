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

    // * Constructors & Deconstructor
    virtual ~GameObject() = default;
    virtual void draw() const = 0;
    
    // * Getters
    btTransform getTransform() const { return this->origin; }

    // * Setters
    void setOrigin(const btVector3& position) {
        this->origin.setIdentity();
        this->origin.setOrigin(position);
    }
    
    // * Functions
    void reset();
    void teleport(const btTransform& t);    
    
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
