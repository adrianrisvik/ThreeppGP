
#ifndef THREEPPGP_MC_HPP
#define THREEPPGP_MC_HPP

#include <threepp/math/Vector3.hpp>
#include <cmath>

class MC {
public:
    explicit MC(const threepp::Vector3 InitialPos = {}, const threepp::Vector3 InitialRot = {})
        : position_(InitialPos), rotation_(InitialRot) {}

    // Movement controls
    void accelerateForward(float dt) {
        currentSpeed_ += acceleration_ * dt;
        if (currentSpeed_ > maxSpeed_) currentSpeed_ = maxSpeed_;
    }

    void accelerateBackward(float dt) {
        currentSpeed_ -= acceleration_ * dt;
        if (currentSpeed_ < -maxSpeed_ * 0.5f) currentSpeed_ = -maxSpeed_ * 0.5f;
    }

    void applyBraking(float dt) {
        if (currentSpeed_ > 0) {
            currentSpeed_ -= braking_ * dt;
            if (currentSpeed_ < 0) currentSpeed_ = 0;
        } else if (currentSpeed_ < 0) {
            currentSpeed_ += braking_ * dt;
            if (currentSpeed_ > 0) currentSpeed_ = 0;
        }
    }

    void applyFriction(float dt) {
        if (currentSpeed_ > 0) {
            currentSpeed_ -= friction_ * dt;
            if (currentSpeed_ < 0) currentSpeed_ = 0;
        } else if (currentSpeed_ < 0) {
            currentSpeed_ += friction_ * dt;
            if (currentSpeed_ > 0) currentSpeed_ = 0;
        }
    }

    void updateMovement(float dt) {
        if (usePhysics_) return; // Bullet handles movement
        float angle = rotation_.y;
        position_.x -= std::sin(angle) * currentSpeed_ * dt;
        position_.z -= std::cos(angle) * currentSpeed_ * dt;
    }

    void turnLeft(float dt) {
        float turnEffectiveness = std::abs(currentSpeed_) / maxSpeed_;
        turnEffectiveness = std::max(0.3f, turnEffectiveness);
        rotation_.y += turn_ * turnEffectiveness * dt;
        targetLean_ = maxLeanAngle_;
    }

    void turnRight(float dt) {
        float turnEffectiveness = std::abs(currentSpeed_) / maxSpeed_;
        turnEffectiveness = std::max(0.3f, turnEffectiveness);
        rotation_.y -= turn_ * turnEffectiveness * dt;
        targetLean_ = -maxLeanAngle_;
    }

    void updateLean(float dt) {
        float leanDiff = targetLean_ - currentLean_;
        currentLean_ += leanDiff * leanSpeed_ * dt;
    }

    // Position & rotation
    threepp::Vector3 getPosition() const { return position_; }
    void setPosition(const threepp::Vector3& p) { position_ = p; }

    threepp::Vector3 getRotation() const { return rotation_; }
    void setRotation(const threepp::Vector3& r) { rotation_ = r; }

    threepp::Vector3 getVisualRotation() const {
        threepp::Vector3 r = rotation_;
        r.y += visualYawOffset_;
        return r;
    }

    // Physics helpers
    threepp::Vector3 getForwardDirection() const {
        return {-std::sin(rotation_.y), 0, -std::cos(rotation_.y)};
    }

    threepp::Vector3 getSpeedVector() const {
        auto dir = getForwardDirection();
        return {dir.x * currentSpeed_, dir.y * currentSpeed_, dir.z * currentSpeed_};
    }

    // Speed & lean
    float getCurrentSpeed() const { return currentSpeed_; }
    void setCurrentSpeed(float speed) { currentSpeed_ = speed; } // Needed for OilSpill & SpeedBoost
    float getCurrentLean() const { return currentLean_; }

    // Config setters
    void setMaxSpeed(float speed) { maxSpeed_ = speed; }
    float getMaxSpeed() const { return maxSpeed_; } // Needed for UI & SpeedBoost
    void setAcceleration(float accel) { acceleration_ = accel; }
    void setBraking(float brake) { braking_ = brake; }
    void setFriction(float friction) { friction_ = friction; }
    void setTurn(float turn) { turn_ = turn; }
    void setMaxLeanAngle(float angle) { maxLeanAngle_ = angle; }
    void setLeanSpeed(float speed) { leanSpeed_ = speed; }

    void resetLean() { targetLean_ = 0; }

    // Visual spin overlay
    void applySpinEffect(float spinAmount) { rotation_.y += spinAmount; }
    void addVisualYaw(float d) { visualYawOffset_ += d; }
    void setVisualYawOffset(float y) { visualYawOffset_ = y; }
    float getVisualYawOffset() const { return visualYawOffset_; }

    // Physics toggle
    void enablePhysics(bool enable) { usePhysics_ = enable; }
    bool isPhysicsEnabled() const { return usePhysics_; }

private:
    float maxSpeed_ = 10.0f;
    float currentSpeed_ = 0.0f;
    float acceleration_ = 5.0f;
    float braking_ = 8.0f;
    float friction_ = 2.0f;
    float turn_ = 2.0f;
    float maxLeanAngle_ = 1.2f;
    float leanSpeed_ = 3.0f;
    float currentLean_ = 0.0f;
    float targetLean_ = 0.0f;
    threepp::Vector3 position_;
    threepp::Vector3 rotation_;
    float visualYawOffset_{0.0f};
    bool usePhysics_ = false;
};

#endif // THREEPPGP_MC_HPP
