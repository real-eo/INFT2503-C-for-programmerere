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

// ---------- Ground ----------
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

// ---------- Sphere ----------
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

// ---------- Cube ----------
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
