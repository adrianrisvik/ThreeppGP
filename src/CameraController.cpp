//
//
// Created by Lenovo on 06.12.2025.
//
#include "CameraController.hpp"
#include <cmath>

CameraController::CameraController(threepp::PerspectiveCamera& camera,
                                   float distanceBehind,
                                   float heightAbove)
    : camera_(camera),
      distanceBehind_(distanceBehind),
      heightAbove_(heightAbove) {
}

void CameraController::update(const MC& target) {
    auto mcPos = target.getPosition();
    auto mcRot = target.getRotation();

    float angle = mcRot.y;

    // Calculate camera position behind the vehicle
    float camX = mcPos.x + distanceBehind_ * std::sin(angle);
    float camZ = mcPos.z + distanceBehind_ * std::cos(angle);
    float camY = mcPos.y + heightAbove_;

    camera_.position.set(camX, camY, camZ);
    camera_.lookAt(mcPos);
}

void CameraController::setDistanceBehind(float distance) {
    distanceBehind_ = distance;
}

void CameraController::setHeightAbove(float height) {
    heightAbove_ = height;
}
