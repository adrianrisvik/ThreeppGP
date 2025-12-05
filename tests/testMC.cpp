#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "MC.hpp"

// ------------------------------------------------------------
// Basic MC physics tests
// ------------------------------------------------------------

TEST_CASE("MC acceleration forward") {
    MC mc;
    mc.setAcceleration(10.0f);
    mc.setMaxSpeed(20.0f);

    mc.accelerateForward(1.0f);   // dt = 1s

    REQUIRE(mc.getCurrentSpeed() == 10.0f);
}

TEST_CASE("MC acceleration forward clamps at maxSpeed") {
    MC mc;
    mc.setAcceleration(50.0f);
    mc.setMaxSpeed(20.0f);

    mc.accelerateForward(1.0f);   // would reach 50, but clamps

    REQUIRE(mc.getCurrentSpeed() == 20.0f);
}

TEST_CASE("MC acceleration backward clamps at half maxSpeed reverse") {
    MC mc;
    mc.setAcceleration(10.0f);
    mc.setMaxSpeed(20.0f);

    mc.accelerateBackward(10.0f); // would be -100, clamp to -10

    REQUIRE(mc.getCurrentSpeed() == -10.0f); // -maxSpeed * 0.5 = -10
}

TEST_CASE("MC braking brings speed to zero") {
    MC mc;
    mc.setBraking(5.f);
    mc.setCurrentSpeed(4.f);

    mc.applyBraking(1.f);

    REQUIRE(mc.getCurrentSpeed() == 0.f);
}

TEST_CASE("MC friction slowly reduces speed") {}
