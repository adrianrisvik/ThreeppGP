//
// Created by Lenovo on 05.12.2025.
//

#ifndef THREEPPGP_PHYSICS_HPP
#define THREEPPGP_PHYSICS_HPP

#include <threepp/math/Vector3.hpp>
#include <threepp/objects/Mesh.hpp>
#include "MC.hpp"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btTriangleMesh.h>

class Physics {

public:
    Physics(float gravity = -9.8f, float groundLevel = 0.0f);

    // Apply physics to an object (e.g., motorcycle)
    void update(MC& mc, float deltaTime);
    // Set velocity externally (e.g., when accelerating)
    void setVelocity(const threepp::Vector3& vel);

    // Get current velocity
    const threepp::Vector3& getVelocity() const;

    // Reset velocity
    void resetVelocity();

private:
    float gravity_;
    float groundLevel_;
    threepp::Vector3 velocity_;
};

#endif //THREEPPGP_PHYSICS_HPP