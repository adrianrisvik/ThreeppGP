
#include "PowerUpManager.hpp"
#include "SpeedBoost.hpp"
#include "OilSpill.hpp"
#include "MC.hpp"
#include <threepp/threepp.hpp>
#include <algorithm>
#include <cmath>


PowerUpManager::PowerUpManager() = default;

void PowerUpManager::addSpeedBoost(const threepp::Vector3& position) {
    auto boost = std::make_unique<SpeedBoost>(position);
    powerUps_.push_back(std::move(boost));
}

void PowerUpManager::addOilSpill(const threepp::Vector3& position) {
    auto oil = std::make_unique<OilSpill>(position);
    powerUps_.push_back(std::move(oil));
}

void PowerUpManager::update(MC& motorcycle, float dt) {
    auto mcPos = motorcycle.getPosition();
    
    for (auto& powerUp : powerUps_) {
        // Check collision
        if (powerUp->checkCollision(mcPos)) {
            powerUp->applyEffect(motorcycle, dt);
        }
        
        // Continue applying effects for duration-based power-ups
        if (powerUp->isEffectActive()) {
            powerUp->applyEffect(motorcycle, dt);
        }
    }
    
    // Remove inactive pickups (not oil spills, they stay)
    powerUps_.erase(
        std::remove_if(powerUps_.begin(), powerUps_.end(),
            [](const std::unique_ptr<PowerUp>& p) {
                return p->getType() == PowerUpType::SpeedBoost && 
                       !p->isActive() && !p->isEffectActive();
            }),
        powerUps_.end()
    );
}

void PowerUpManager::addToScene(threepp::Scene& scene) {
    for (auto& powerUp : powerUps_) {
        if (auto* mesh = powerUp->getMesh()) {
            scene.add(std::shared_ptr<threepp::Object3D>(mesh));
        }
    }
}

void PowerUpManager::animate(float time) {
    for (auto& powerUp : powerUps_) {
        if (powerUp->isActive() && powerUp->getType() == PowerUpType::SpeedBoost) {
            auto* mesh = powerUp->getMesh();
            if (mesh) {
                mesh->rotation.z = time * 2.0f;  // Spin the speed boost
                mesh->position.y = std::sin(time * 3.0f) * 0.2f + 0.5f;  // Bob up and down
            }
        }
    }
}

const std::vector<std::unique_ptr<PowerUp>>& PowerUpManager::getPowerUps() const {
    return powerUps_;
}