//
// Created by Lenovo on 17.11.2025.
//

#include "../include/Vehicle.hpp"

Vehicle::Vehicle(std::shared_ptr<threepp::Group> model, const VehicleStats& stats)
                : model_(model), stats_(stats) {
}

void Vehicle::updateTransform(const threepp::Vector3& position, const threepp::Vector3& rotation, float lean) {
    if (model_) return;

    model_->position.copy(position);
    model_->rotation.setFromVector3(rotation);
    model_->rotation.z += lean;
}