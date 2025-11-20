//
// Created by Lenovo on 17.11.2025.
//

#ifndef THREEPPGP_VEHICLE_HPP
#define THREEPPGP_VEHICLE_HPP

#include <threepp/threepp.hpp>
#include <memory>
#include "VehicleStats.hpp"

class Vehicle {
public:
    Vehicle(std::shared_ptr<threepp::Group> model, const VehicleStats& stats);

    void updateTransform(const threepp::Vector3& position,
                        const threepp::Vector3& rotation,
                        float lean);

    void setInitialRotation(const threepp::Vector3& rotation) {
        initialRotation_ = rotation;
    }

    std::shared_ptr<threepp::Group> getModel() const { return model_; }
    const VehicleStats& getStats() const { return stats_; }

private:
    std::shared_ptr<threepp::Group> model_;
    VehicleStats stats_;
    threepp::Vector3 initialRotation_{0, 0, 0};
};

#endif //THREEPPGP_VEHICLE_HPP