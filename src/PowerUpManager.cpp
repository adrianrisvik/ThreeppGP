//
// Created by Lenovo on 06.12.2025.
//

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

        // Continuous effect in it's duration
        if (powerUp->isEffectActive()) {
            powerUp->applyEffect(motorcycle, dt);
        }
    }

    // Remove speed boost after us, oil spill stays for multiple uses
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
        auto mesh = powerUp->getMesh();  // Returns shared_ptr now
        if (mesh) {
            scene.add(mesh);  // Add the shared_ptr directly
        }
    }
}

void PowerUpManager::animate(float time) {
    for (auto& powerUp : powerUps_) {
        if (powerUp->isActive() && powerUp->getType() == PowerUpType::SpeedBoost) {
            auto mesh = powerUp->getMesh();  // Returns shared_ptr now
            if (mesh) {
                mesh->rotation.z = time * 2.0f;
                mesh->position.y = std::sin(time * 3.0f) * 0.2f + 0.5f;
            }
        }
    }
}

const std::vector<std::unique_ptr<PowerUp>>& PowerUpManager::getPowerUps() const {
    return powerUps_;
}