//
// Created by Lenovo on 06.12.2025.
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
    // WASD controls
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

    }
    // Key release
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

        // Update position
        mc.updateMovement(dt);

        // Turning
        if (isAPressed) {
            mc.turnLeft(dt);
            isTurning = true;
        }
        if (isDPressed) {
            mc.turnRight(dt);
            isTurning = true;
        }

        // Reset lean when not turning
        if (!isTurning) {
            mc.resetLean();
        }

        // Update lean animation
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