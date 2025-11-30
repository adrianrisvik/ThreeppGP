#ifndef THREEPPGP_OILSPILL_HPP
#define THREEPPGP_OILSPILL_HPP

#include "PowerUp.hpp"


class MC;

class OilSpill : public PowerUp {
public:
    OilSpill(const threepp::Vector3& position, float radius = 2.0f,
             float spinForce = 6.0f, float speedReduction = 0.5f,
             float duration = 1.0f);

    void applyEffect(MC& motorcycle, float dt) override;
    bool isEffectActive() const override;

private:
    void createMesh();

    float spinForce_;
    float speedReduction_;
    bool hasHit_;
    float duration_;
    float timeRemaining_{0.0f};
    float originalFriction_{0.0f};
    float originalTurn_{0.0f};
    float originalMaxSpeed_{0.0f};
    float angularVelocity_{0.0f};
};

#endif // THREEPPGP_OILSPILL_HPP