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
        // Effect is ongoing, count down
        timeRemaining_ -= dt;
        if (timeRemaining_ <= 0.0f) {
            // Restore original speed
            motorcycle.setMaxSpeed(originalMaxSpeed_);
        }
    } else {
        // First time pickup
        active_ = false;  // Mark as collected
        timeRemaining_ = duration_;
        originalMaxSpeed_ = motorcycle.getMaxSpeed();
        motorcycle.setMaxSpeed(originalMaxSpeed_ * speedMultiplier_);
    }
}

bool SpeedBoost::isEffectActive() const {
    return !active_ && timeRemaining_ > 0.0f;
}

void SpeedBoost::createMesh() {
    // Create a rotating ring/star shape for speed boost
    auto geometry = threepp::TorusGeometry::create(0.5f, 0.2f, 8, 16);
    auto material = threepp::MeshPhongMaterial::create();
    material->color = threepp::Color(0xFFFF00);  // Yellow - use hex constructor
    material->emissive = threepp::Color(0xFFA500);  // Orange - use hex constructor
    material->emissiveIntensity = 0.5f;

    mesh_ = threepp::Mesh::create(geometry, material);
    mesh_->position.copy(position_);
    mesh_->rotation.x = threepp::math::PI / 2;  // Lay flat on ground
}