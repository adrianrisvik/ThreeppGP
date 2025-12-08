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

    // Add power-ups
    void addSpeedBoost(const threepp::Vector3& position);
    void addOilSpill(const threepp::Vector3& position);

    // Update powerups and check for collisions
    void update(MC& motorcycle, float dt);

    // Add powerup meshes to scene
    void addToScene(threepp::Scene& scene);

    void animate(float time);

    const std::vector<std::unique_ptr<PowerUp>>& getPowerUps() const;

private:
    std::vector<std::unique_ptr<PowerUp>> powerUps_;
};

#endif // THREEPPGP_POWERUPMANAGER_HPP