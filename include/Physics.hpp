
#ifndef THREEPPGP_PHYSICS_HPP
#define THREEPPGP_PHYSICS_HPP

#include <threepp/math/Vector3.hpp>
#include "MC.hpp"
#include <btBulletDynamicsCommon.h>

class Physics {
public:
    Physics(float gravity, float groundLevel, MC& mc);
    ~Physics();

    void update(MC& mc, float deltaTime);
    void setVelocity(const threepp::Vector3& vel);
    const threepp::Vector3& getVelocity() const;
    void resetVelocity();
    void applyAcceleration(MC& mc);

private:
    float gravity_;
    float groundLevel_;
    threepp::Vector3 velocity_;

    btBroadphaseInterface* broadphase_;
    btDefaultCollisionConfiguration* collisionConfig_;
    btCollisionDispatcher* dispatcher_;
    btSequentialImpulseConstraintSolver* solver_;
    btDiscreteDynamicsWorld* dynamicsWorld_;
    btRigidBody* mcRigidBody_;
};

#endif
