
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "PowerUp.hpp"
#include "SpeedBoost.hpp"
#include "OilSpill.hpp"
#include <threepp/math/Vector3.hpp>


// Dummy MC class for testing applyEffect signature
class MC {};

TEST_CASE("PowerUp base functionality", "[PowerUp]") {
    threepp::Vector3 pos{0, 0, 0};
    SpeedBoost sb(pos);
    OilSpill os(pos);

    SECTION("Initial state") {
        REQUIRE(sb.isActive());
        REQUIRE(os.isActive());
        REQUIRE(sb.getType() == PowerUpType::SpeedBoost);
        REQUIRE(os.getType() == PowerUpType::OilSpill);
    }

    SECTION("Collision detection") {
        threepp::Vector3 mcPos{0.5f, 0, 0.5f};
        REQUIRE(sb.checkCollision(mcPos)); // within radius
        REQUIRE(os.checkCollision(mcPos)); // within radius

        threepp::Vector3 farPos{10, 0, 10};
        REQUIRE_FALSE(sb.checkCollision(farPos));
        REQUIRE_FALSE(os.checkCollision(farPos));
    }

    SECTION("Deactivate power-up") {
        sb.setActive(false);
        REQUIRE_FALSE(sb.isActive());
        REQUIRE_FALSE(sb.checkCollision({0, 0, 0}));
    }
}

TEST_CASE("Derived classes applyEffect signature", "[PowerUp]") {
    threepp::Vector3 pos{0, 0, 0};
    SpeedBoost sb(pos);
    OilSpill os(pos);
    MC dummyMC;

    REQUIRE_NOTHROW(sb.applyEffect(dummyMC, 0.016f));
    REQUIRE_NOTHROW(os.applyEffect(dummyMC, 0.016f));
}
