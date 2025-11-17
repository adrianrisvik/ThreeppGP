#ifndef THREEPPGP_OILSPILL_HPP
#define THREEPPGP_OILSPILL_HPP

#include "PowerUp.hpp"
#include <threepp/threepp.hpp>

class MC;

class OilSpill : public PowerUp {
public:
    OilSpill(const threepp::Vector3& position, float radius = 2.0f,
             float spinForce = 3.0f, float speedReduction = 0.3f);

    void applyEffect(MC& motorcycle, float dt) override;
    bool isEffectActive() const override;

private:
    void createMesh();

    float spinForce_;
    float speedReduction_;
    bool hasHit_;
};

#endif // THREEPPGP_OILSPILL_HPP