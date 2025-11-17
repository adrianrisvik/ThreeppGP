#include "threepp/threepp.hpp"
#include "OilSpill.hpp"
#include "MC.hpp"
#include <random>

OilSpill::OilSpill(const threepp::Vector3& position, float radius,
                   float spinForce, float speedReduction)
    : PowerUp(position, PowerUpType::OilSpill, radius),
      spinForce_(spinForce),
      speedReduction_(speedReduction),
      hasHit_(false) {
    createMesh();
}

void OilSpill::applyEffect(MC& motorcycle, float dt) {
    if (!hasHit_) {
        hasHit_ = true;
        
        // Reduce speed dramatically
        float currentSpeed = motorcycle.getCurrentSpeed();
        
        // Force a spin by applying rotation
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-1.0, 1.0);
        
        float spinDirection = dis(gen) > 0 ? 1.0f : -1.0f;
        
        // Apply spin effect
        motorcycle.applySpinEffect(spinForce_ * spinDirection);
        
        // Reduce max speed temporarily
        motorcycle.setMaxSpeed(motorcycle.getMaxSpeed() * speedReduction_);
    }
}

bool OilSpill::isEffectActive() const {
    return active_;  // Oil spill stays active, can hit multiple times
}

void OilSpill::createMesh() {
    // Create a dark puddle shape
    auto geometry = threepp::CylinderGeometry::create(radius_, radius_, 0.05f, 32);
    auto material = threepp::MeshPhongMaterial::create();
    material->color = threepp::Color(0x2a2a2a);  // Dark grey/black
    material->transparent = true;
    material->opacity = 0.8f;
    material->shininess = 100.0f;  // Make it look wet/slippery
    
    mesh_ = threepp::Mesh::create(geometry, material);
    mesh_->position.copy(position_);
    mesh_->position.y = 0.05f;  // Slightly above ground
}