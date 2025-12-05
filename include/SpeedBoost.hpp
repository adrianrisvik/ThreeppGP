#ifndef THREEPPGP_SPEEDBOOST_HPP
#define THREEPPGP_SPEEDBOOST_HPP

#include "PowerUp.hpp"

// Forward declarations instead of full include
namespace threepp {
    class Vector3;
}

class MC; // Forward declaration

class SpeedBoost : public PowerUp {
public:
    SpeedBoost(const threepp::Vector3& position, float radius = 1.0f,
                float speedMultiplier = 1.5f, float duration = 5.0f);

    void applyEffect(MC& motorcycle, float dt) override;
    bool isEffectActive() const override;

private:
    void createMesh();

    float speedMultiplier_;
    float duration_;
    float timeRemaining_;
    float originalMaxSpeed_;
};

#endif // THREEPPGP_SPEEDBOOST_HPP