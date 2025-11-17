//
// Created by Lenovo on 17.11.2025.
//

#include "VehicleManager.hpp"
#include <iostream>

void VehicleManager::addVehicle(std::shared_ptr<threepp::Group> model, const VehicleStats& stats) {
    vehicles_.emplace_back(model, stats);
}

bool VehicleManager::switchVehicle(int index, threepp::Scene& scene, MC& mc) {
    if (index < 0 || index >= vehicles_.size() || index == currentIndex_) {
        return false;
    }

    scene.remove(*vehicles_[currentIndex_].getModel());

    currentIndex_ = index;
    const auto& newVehicle = vehicles_[currentIndex_];

    scene.add(newVehicle.getModel());

    const auto& stats = newVehicle.getStats();
    mc.setMaxSpeed(stats.maxSpeed);
    mc.setAcceleration(stats.acceleration);
    mc.setBraking(stats.braking);
    mc.setFriction(stats.friction);
    mc.setTurn(stats.turn);

    std::cout <<"Switched to: " <<stats.name << std::endl;

    return true;
}

void VehicleManager::updateCurrentVehicle(const threepp::Vector3& position,
                                          const threepp::Vector3& rotation,
                                          float lean) {
    if (!vehicles_.empty()) {
        vehicles_[currentIndex_].updateTransform(position, rotation, lean);
    }
}
void VehicleManager::addInitialVehicleToScene(threepp::Scene& scene) {
    if (!vehicles_.empty()) {
        scene.add(vehicles_[currentIndex_].getModel());
    }
}
    
