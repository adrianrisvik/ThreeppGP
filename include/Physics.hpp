//
// Created by Lenovo on 06.12.2025.
//
#ifndef THREEPPGP_PHYSICS_HPP
#define THREEPPGP_PHYSICS_HPP

#include <threepp/math/Vector3.hpp>
#include "MC.hpp"
#include <btBulletDynamicsCommon.h>
#include <string>

class Physics {
public:
    Physics(float gravity, MC& mc);
    ~Physics();

    void update(MC& mc, float dt);
    void setVelocity(const threepp::Vector3& vel);
    const threepp::Vector3& getVelocity() const;
    void resetVelocity();
    void applyAcceleration(MC& mc);
    void setGravity(float g);
    void setMass(MC& mc, float m);
    void setFriction(float f);
    void setDamping(float linear, float angular);
    void setSubsteps(int steps);
    void resetRigidBodyPosition(const MC& mc);

private:
    btRigidBody* createStaticCollisionShapeFromObj(const std::string& path);

    float gravity_;
    //float groundLevel_;
    threepp::Vector3 velocity_;

    btBroadphaseInterface* broadphase_;
    btDefaultCollisionConfiguration* collisionConfig_;
    btCollisionDispatcher* dispatcher_;
    btSequentialImpulseConstraintSolver* solver_;
    btDiscreteDynamicsWorld* dynamicsWorld_;
    btRigidBody* mcRigidBody_;
};

#endif
