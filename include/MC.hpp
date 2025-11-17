//
// Created by Lenovo on 11.11.2025.
//

#ifndef THREEPPGP_MC_HPP
#define THREEPPGP_MC_HPP

#include <threepp/math/Vector3.hpp>
#include <cmath>

class MC {
public:
    explicit MC(const threepp::Vector3 InitialPos = {})
    : position_(InitialPos) {
    }

    void accelerateForward(float dt) {
        currentSpeed_ += acceleration_ * dt;
        if (currentSpeed_ > maxSpeed_) {
            currentSpeed_ = maxSpeed_;
        }
    }

    void accelerateBackward(float dt) {
        currentSpeed_ -= acceleration_ * dt;
        if (currentSpeed_ < -maxSpeed_ * 0.5f) {  // Reverse is typically slower
            currentSpeed_ = -maxSpeed_ * 0.5f;
        }
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
        // Natural deceleration when no input
        if (currentSpeed_ > 0) {
            currentSpeed_ -= friction_ * dt;
            if (currentSpeed_ < 0) currentSpeed_ = 0;
        } else if (currentSpeed_ < 0) {
            currentSpeed_ += friction_ * dt;
            if (currentSpeed_ > 0) currentSpeed_ = 0;
        }
    }

    void updateMovement(float dt) {
        // Move based on current speed
        float angle = rotation_.y;
        position_.x -= std::sin(angle) * currentSpeed_ * dt;
        position_.z -= std::cos(angle) * currentSpeed_ * dt;
    }

    void turnLeft(float dt) {
        // Turning is more effective at higher speeds
        float turnEffectiveness = std::abs(currentSpeed_) / maxSpeed_;
        turnEffectiveness = std::max(0.3f, turnEffectiveness);  // Minimum turn rate
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

    threepp::Vector3 getPosition() {
        return position_;
    }

    threepp::Vector3 getRotation() {
        return rotation_;
    }

    float getCurrentSpeed() const {
        return currentSpeed_;
    }

    float getCurrentLean() const {
        return currentLean_;
    }

    void setMaxSpeed(float speed) {
        maxSpeed_ = speed;
    }

    void setAcceleration(float accel) {
        acceleration_ = accel;
    }

    void setBraking(float brake) {
        braking_ = brake;
    }

    void setFriction(float friction) {
        friction_ = friction;
    }

    void setTurn(float turn) {
        turn_ = turn;
    }

    void setMaxLeanAngle(float angle) {
        maxLeanAngle_ = angle;
    }

    void setLeanSpeed(float speed) {
        leanSpeed_ = speed;
    }

    void resetLean() {
        targetLean_ = 0;
    }
    float getMaxSpeed() const {
        return maxSpeed_;
    }

    void applySpinEffect(float spinAmount) {
        rotation_.y += spinAmount;
    }

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
};



#endif //THREEPPGP_MC_HPP