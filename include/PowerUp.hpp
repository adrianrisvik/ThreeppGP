#ifndef THREEPPGP_POWERUP_HPP
#define THREEPPGP_POWERUP_HPP

#include <threepp/math/Vector3.hpp>
#include <threepp/objects/Mesh.hpp>
#include <memory>

class MC;

enum class PowerUpType {
    SpeedBoost,
    OilSpill
};

class PowerUp {
public:
    PowerUp(const threepp::Vector3& position, PowerUpType type, float radius = 1.0f)
        : position_(position), type_(type), radius_(radius), active_(true) {}

    virtual ~PowerUp() = default;

    // Called when motorcycle collides with this power-up
    virtual void applyEffect(MC& motorcycle, float dt) = 0;

    // Check if effect is still active
    virtual bool isEffectActive() const { return active_; }

    // Check collision with motorcycle
    bool checkCollision(const threepp::Vector3& mcPosition) const {
        if (!active_) return false;
        float dx = position_.x - mcPosition.x;
        float dz = position_.z - mcPosition.z;
        float distanceSquared = dx * dx + dz * dz;
        return distanceSquared <= (radius_ * radius_);
    }

    threepp::Vector3 getPosition() const { return position_; }
    PowerUpType getType() const { return type_; }
    bool isActive() const { return active_; }
    void setActive(bool active) { active_ = active; }

    // Get the visual mesh for rendering
    std::shared_ptr<threepp::Mesh> getMesh() { return mesh_; }

protected:
    threepp::Vector3 position_;
    PowerUpType type_;
    float radius_;
    bool active_;
    std::shared_ptr<threepp::Mesh> mesh_;
};

#endif // THREEPPGP_POWERUP_HPP