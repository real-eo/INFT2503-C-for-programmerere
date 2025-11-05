#include "gameObjects/gameObjects.hpp"

#include <SFML/OpenGL.hpp>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif


// Small fix due to M_PI being omitted when compiling using strict standard flags on Windows MinGW and Windows MSYS2
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


extern "C" {
    // Implementation is in glut_shapes.c
    void glutSolidCube(GLdouble size);
}


// * -------- GameObject --------
std::array<float*, 4U> GameObject::getPositionPointers() {
    // ? Steps to get the non-const pointers to the individual btScalars within the btVector3 m_origin member of btTransform:
    /* // * 1)
    btTransform (class) {
        ///Storage for the rotation
        btMatrix3x3 m_basis;                            // ! Private by default since btTransform is a class; - i.e. inaccessible
        
        ///Storage for the translation
        btVector3 m_origin;                             // ! Private by default since btTransform is a class; - i.e. inaccessible

    public:
        btVector3& getOrigin() { return m_origin; }     // | Public non-const reference getter
    }
    */
    
    /* // * 2)
    btVector3 (class) { 
    public:
        btScalar m_floats[4];                           // | Public member array of 4 btScalars, accessible directly via the class instance
    }
    */

    /* // * 3)
    typedef float btScalar                              // | btScalar is just a typedef of float
    */


    // Get the non-const reference to the m_origin member from the btTransform class 
    btVector3& m_originRef = position.getOrigin();

    // Return pointers to the four btScalars within the btVector3's m_floats array. This must be 
    // done by us, as the only getters within the class are either const, or passed by value.
    // When returning, treat values as floats so that they get correctly interpreted by ImGui.
    // This can be done safely as btScalar is just a typedef of float.
    return {
        &m_originRef.m_floats[0],
        &m_originRef.m_floats[1],
        &m_originRef.m_floats[2],
        &m_originRef.m_floats[3]
    };
}

void GameObject::reset() {
    // Reset velocities
    body->setLinearVelocity(btVector3(0, 0, 0));
    body->setAngularVelocity(btVector3(0, 0, 0));

    // Clear accumulated forces
    body->clearForces();

    // Reset positions
    body->setCenterOfMassTransform(getOrigin() * position);                             // ? We multiply by the position to include any positional offsets set by the user
    motion_state->setWorldTransform(getOrigin() * position);                            // ? Same here

    // Wake up the body to ensure it processes the reset
    body->activate(true);
}

// Make a GameObject kinematic; - not affected by physics
void GameObject::kinematic() {
    body->setCollisionFlags(
        body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT
    );
    
    body->setActivationState(DISABLE_DEACTIVATION);
}

// Make a GameObject dynamic; - affected by physics
void GameObject::dynamic() {
    body->setCollisionFlags(
        body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT
    );

    body->forceActivationState(ACTIVE_TAG);
}



// * ---------- Ground ----------
Ground::Ground()
: shape(btVector3(0.0, 1.0, 0.0), 0.0) {
    motion_state = std::make_unique<btDefaultMotionState>();

    body = std::make_unique<btRigidBody>(0.0, motion_state.get(), &shape);

    body->setRollingFriction(0.1);
    body->setRestitution(0.8);
}

void Ground::draw() const {
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



// * ---------- Sphere ----------
Sphere::Sphere()
: shape(btScalar(0.1)) {
    motion_state = std::make_unique<btDefaultMotionState>();
    body = std::make_unique<btRigidBody>(1.0, motion_state.get(), &shape);

    auto mass = 1.0 / body->getInvMass();

    btVector3 inertia;

    shape.calculateLocalInertia(mass, inertia);
    body->setMassProps(mass, inertia);
    
    body->setRollingFriction(0.2);
    body->setRestitution(0.8);
}

void Sphere::draw() const {
    glPushMatrix();

    auto position = body->getCenterOfMassPosition();
    glTranslatef(position.x(), position.y(), position.z());

    glColor3f(0.0, 0.0, 1.0);

    auto quadric = gluNewQuadric();
    gluSphere(quadric, shape.getRadius(), 32, 32);
    gluDeleteQuadric(quadric);

    glPopMatrix();
}



// * ---------- Cube ----------
Cube::Cube()
: shape({0.1, 0.1, 0.1}) {
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

void Cube::draw() const {
    glPushMatrix();
    
    auto position = body->getCenterOfMassPosition();
    glTranslatef(position.x(), position.y(), position.z());
    
    auto q = body->getOrientation();
    btVector3 axis = q.getAxis();

    if (!axis.fuzzyZero()) axis.normalize();

    glRotatef(q.getAngle() * 180.0 / M_PI, axis.x(), axis.y(), axis.z());
    glColor3f(1.0, 0.0, 0.0);
    glutSolidCube(0.2);

    glPopMatrix();
}
