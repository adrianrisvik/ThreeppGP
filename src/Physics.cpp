//
// Created by Lenovo on 05.12.2025.
//

#include "Physics.hpp"


Physics::Physics(float gravity, float groundLevel)
    : gravity_(gravity), groundLevel_(groundLevel), velocity_(0, 0, 0) {}


void Physics::update(MC& mc, float deltaTime) {
    // Apply gravity
    velocity_.y += gravity_ * deltaTime;

    // Update MC position
    auto pos = mc.getPosition();
    pos.y += velocity_.y * deltaTime;

    // Simple ground collision
    if (pos.y <= groundLevel_) {
        pos.y = groundLevel_;
        velocity_.y = 0;
    }

    mc.setPosition(pos);
}
