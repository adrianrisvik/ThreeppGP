//
// Created by Lenovo on 17.11.2025.
//

#ifndef THREEPPGP_VEHICLESTATS_HPP
#define THREEPPGP_VEHICLESTATS_HPP

#include <string>




struct VehicleStats {
    float maxSpeed;
    float acceleration;
    float braking;
    float friction;
    float turn;
    std::string name;

    VehicleStats(float maxSpeed,
                 float acceleration,
                 float braking,
                 float friction,
                 float turn,
                 std::string name)
               : maxSpeed(maxSpeed),
                 acceleration(acceleration),
                 braking(braking),
                 friction(friction),
                 turn(turn),
                 name(name) {}
};


#endif //THREEPPGP_VEHICLESTATS_HPP