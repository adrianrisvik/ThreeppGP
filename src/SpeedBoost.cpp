#include "threepp/threepp.hpp"
#include "SpeedBoost.hpp"
#include "MC.hpp"


SpeedBoost::SpeedBoost(const threepp::Vector3& position, float radius,
                       float speedMultiplier, float duration)
    : PowerUp(position, PowerUpType::SpeedBoost, radius),
      speedMultiplier_(speedMultiplier),
      duration_(duration),
      timeRemaining_(0.0f),
      originalMaxSpeed_(0.0f)
{
    createMesh();
}

void SpeedBoost::applyEffect(MC& motorcycle, float dt) {
    if (!active_) {
        // Effect is ongoing, count down (no extra visuals during boost)
        timeRemaining_ -= dt;
        if (timeRemaining_ <= 0.0f) {
            // Restore original speed
            motorcycle.setMaxSpeed(originalMaxSpeed_);
            // Mesh remains hidden; manager will remove this pickup later
        }
    } else {
        // First time pickup
        active_ = false;  // Mark as collected
        timeRemaining_ = duration_;
        originalMaxSpeed_ = motorcycle.getMaxSpeed();
        const float boostedMax = originalMaxSpeed_ * speedMultiplier_;
        motorcycle.setMaxSpeed(boostedMax);
        // Kick an immediate speed impulse so the boost feels a lot faster right away
        // Aim for ~85% of boosted max speed (but do not exceed it)
        const float targetSpeed = boostedMax * 0.85f;
        if (motorcycle.getCurrentSpeed() < targetSpeed) {
            motorcycle.setCurrentSpeed(targetSpeed);
        }
        // Hide the pickup mesh immediately for a cleaner look
        if (mesh_) mesh_->visible = false;
    }
}

bool SpeedBoost::isEffectActive() const {
    return !active_ && timeRemaining_ > 0.0f;
}

void SpeedBoost::createMesh() {
    // Create a rotating ring/star shape for speed boost
    auto geometry = threepp::TorusGeometry::create(0.5f, 0.2f, 8, 16);
    auto material = threepp::MeshPhongMaterial::create();
    material->color = threepp::Color(0x800080);  // Yellow - use hex constructor
    material->emissive = threepp::Color(0xFFA500);  // Orange - use hex constructor
    material->emissiveIntensity = 0.5f;

    mesh_ = threepp::Mesh::create(geometry, material);
    mesh_->position.copy(position_);
    mesh_->rotation.x = threepp::math::PI / 2;  // Lay flat on ground
}