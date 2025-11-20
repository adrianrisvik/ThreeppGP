//
// Created by Lenovo on 17.11.2025.
//

#include "Vehicle.hpp"
#include <cmath>

Vehicle::Vehicle(std::shared_ptr<threepp::Group> model, const VehicleStats& stats)
    : model_(std::move(model)), stats_(stats) {
}

void Vehicle::updateTransform(const threepp::Vector3& position, 
                             const threepp::Vector3& rotation, 
                             float lean) {
    if (!model_) return;
    
    // Set position directly
    model_->position.x = position.x;
    model_->position.y = position.y;
    model_->position.z = position.z;
    
    // Apply initial rotation offset + current rotation
    model_->rotation.x = initialRotation_.x + rotation.x;
    model_->rotation.y = initialRotation_.y + rotation.y;

    // Simple, uniform lean for all vehicles: apply roll around Z only
    model_->rotation.z = initialRotation_.z + rotation.z + lean;
}