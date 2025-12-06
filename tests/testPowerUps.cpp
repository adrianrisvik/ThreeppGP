
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "PowerUp.hpp"
#include "SpeedBoost.hpp"
#include "OilSpill.hpp"
#include <threepp/math/Vector3.hpp>
#include "MC.hpp"

TEST_CASE("PowerUp base functionality", "[PowerUp]") {
    threepp::Vector3 pos{0, 0, 0};
    SpeedBoost sb(pos);
    OilSpill os(pos);
    MC dummyMC;

    REQUIRE_NOTHROW(sb.applyEffect(dummyMC, 0.016f));
    REQUIRE_NOTHROW(os.applyEffect(dummyMC, 0.016f));


    SECTION("Initial state") {
        REQUIRE_FALSE(sb.isActive()); // If default is inactive
        REQUIRE(os.isActive());
    }



    SECTION("Collision detection") {
        threepp::Vector3 mcPos{0.5f, 0, 0.5f};
        REQUIRE_FALSE(sb.checkCollision(mcPos)); // If inactive or radius is smaller
        REQUIRE(os.checkCollision(mcPos));
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
