//
// Created by Lenovo on 11.11.2025.
//

#ifndef THREEPPGP_MCKEYCONTROLLER_HPP
#define THREEPPGP_MCKEYCONTROLLER_HPP

#include <threepp/input/KeyListener.hpp>
#include "MC.hpp"

class MCKeyController : public threepp::KeyListener {
public:
    bool enabled = true;
    std::function<void(int)> onVehicleChange;

    explicit MCKeyController(MC &mc) : mc(mc) {}

    void onKeyPressed(threepp::KeyEvent evt)override {
        if (evt.key == threepp::Key::W) {
            isWPressed = true;
        }
        else if (evt.key == threepp::Key::A) {
            isAPressed = true;
        }
        else if (evt.key == threepp::Key::S) {
            isSPressed = true;
        }
        else if (evt.key == threepp::Key::D) {
            isDPressed = true;
        }
        else if (evt.key == threepp::Key::SPACE) {
            isSpacePressed = true;
        }
        else if (evt.key == threepp::Key::NUM_1) {
            if (onVehicleChange) onVehicleChange(0);
        }
        else if (evt.key == threepp::Key::NUM_2) {
            if (onVehicleChange) onVehicleChange(1);
        }
        else if (evt.key == threepp::Key::NUM_3) {
            if (onVehicleChange) onVehicleChange(2);
        }
        else if (evt.key == threepp::Key::NUM_4) {
            if (onVehicleChange) onVehicleChange(3);
        }
    }

    void onKeyReleased(threepp::KeyEvent evt)override {
        if (evt.key == threepp::Key::W) {
            isWPressed = false;
        }
        else if (evt.key == threepp::Key::A) {
            isAPressed = false;
        }
        else if (evt.key == threepp::Key::S) {
            isSPressed = false;
        }
        else if (evt.key == threepp::Key::D) {
            isDPressed = false;
        }
        else if (evt.key == threepp::Key::SPACE) {
            isSpacePressed = false;
        }

    }

    void update(float dt) {
        if (!enabled) return;

        bool isTurning = false;
        bool isAccelerating = false;

        // Handle acceleration
        if (isWPressed) {
            mc.accelerateForward(dt);
            isAccelerating = true;
        }

        // Handle reverse
        if (isSPressed) {
            mc.accelerateBackward(dt);
            isAccelerating = true;
        }

        // Handle active braking with spacebar
        if (isSpacePressed) {
            mc.applyBraking(dt);
        }
        // Apply friction when not accelerating and not braking
        else if (!isAccelerating) {
            mc.applyFriction(dt);
        }

        // Update position based on current speed
        mc.updateMovement(dt);

        // Handle turning
        if (isAPressed) {
            mc.turnLeft(dt);
            isTurning = true;
        }
        if (isDPressed) {
            mc.turnRight(dt);
            isTurning = true;
        }

        // If not turning, reset lean to upright
        if (!isTurning) {
            mc.resetLean();
        }

        // Always update lean animation
        mc.updateLean(dt);
    }

private:
    MC& mc;
    bool isWPressed = false;
    bool isAPressed = false;
    bool isSPressed = false;
    bool isDPressed = false;
    bool isSpacePressed = false;
};


#endif //THREEPPGP_MCKEYCONTROLLER_HPP