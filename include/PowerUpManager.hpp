//
// Created by Lenovo on 06.12.2025.
//

#ifndef THREEPPGP_POWERUPMANAGER_HPP
#define THREEPPGP_POWERUPMANAGER_HPP

#include "PowerUp.hpp"
#include <vector>
#include <memory>

namespace threepp {
    class Scene;
}

class MC;

class PowerUpManager {
public:
    PowerUpManager();

    // Add power-ups to the world
    void addSpeedBoost(const threepp::Vector3& position);
    void addOilSpill(const threepp::Vector3& position);

    // Update all power-ups and check collisions
    void update(MC& motorcycle, float dt);

    // Add all power-up meshes to the scene
    void addToScene(threepp::Scene& scene);

    // Animate power-ups (rotation, bobbing, etc.)
    void animate(float time);

    const std::vector<std::unique_ptr<PowerUp>>& getPowerUps() const;

private:
    std::vector<std::unique_ptr<PowerUp>> powerUps_;
};

#endif // THREEPPGP_POWERUPMANAGER_HPP