//
// Created by Lenovo on 16.11.2025.
//

#ifndef THREEPPGP_CAMERA_HPP
#define THREEPPGP_CAMERA_HPP

#include <threepp/threepp.hpp>
#include "MC.hpp"

class CameraController {
public:
    CameraController(threepp::PerspectiveCamera& camera, 
                     float distanceBehind = 8.0f,
                     float heightAbove = 3.5f);

    // Update camera position and orientation based on target
    void update(const MC& target);

    // Setters for camera parameters
    void setDistanceBehind(float distance);
    void setHeightAbove(float height);

    // Getters
    float getDistanceBehind() const { return distanceBehind_; }
    float getHeightAbove() const { return heightAbove_; }

private:
    threepp::PerspectiveCamera& camera_;
    float distanceBehind_;
    float heightAbove_;
};

#endif // THREEPPGP_CAMERA_HPP