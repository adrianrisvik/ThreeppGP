//
// Created by Lenovo on 06.12.2025.
//

#include "threepp/threepp.hpp"
#include "OilSpill.hpp"
#include "MC.hpp"
#include <random>

OilSpill::OilSpill(const threepp::Vector3& position, float radius,
                   float spinForce, float speedReduction, float duration)
    : PowerUp(position, PowerUpType::OilSpill, radius),
      spinForce_(spinForce),
      speedReduction_(speedReduction),
      hasHit_(false),
      duration_(duration) {
    createMesh();
}

void OilSpill::applyEffect(MC& motorcycle, float dt) {

    if (timeRemaining_ > 0.0f) {
        // Visual-only spin around its own vertical axis: do not change heading/trajectory
        motorcycle.addVisualYaw(angularVelocity_ * dt);
        timeRemaining_ -= dt;
        if (timeRemaining_ <= 0.0f) {
            // End of spin
            motorcycle.setVisualYawOffset(0.0f);
            hasHit_ = false; // Allow hitting again
            timeRemaining_ = 0.0f;
        }
        return;
    }

    if (!hasHit_) {
        hasHit_ = true;

        // Randomize spin direction
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);
        float spinDirection = dis(gen) > 0.5f ? 1.0f : -1.0f;

        // 2 spins per hit
        float totalAngle = 2.0f * 2.0f * threepp::math::PI; // 720 degrees
        angularVelocity_ = (totalAngle / duration_) * spinDirection; // rad/s
        timeRemaining_ = duration_;

        // Speed reduction
        motorcycle.setCurrentSpeed(motorcycle.getCurrentSpeed() * speedReduction_);
    }
}

bool OilSpill::isEffectActive() const {
    return timeRemaining_ > 0.0f;
}

void OilSpill::createMesh() {
    // Visual design with help from AI
    auto geometry = threepp::CylinderGeometry::create(radius_, radius_, 0.05f, 32);
    auto material = threepp::MeshPhongMaterial::create();
    material->color = threepp::Color(0x2a2a2a);  // Dark grey/black
    material->transparent = true;
    material->opacity = 0.8f;
    material->shininess = 100.0f;
    
    mesh_ = threepp::Mesh::create(geometry, material);
    mesh_->position.copy(position_);
    mesh_->position.y = 0.05f;  // Slightly above ground to guarantee being seen
}