//
// Created by Lenovo on 17.11.2025.
//

#ifndef THREEPPGP_VEHICLEMANAGER_HPP
#define THREEPPGP_VEHICLEMANAGER_HPP

#include <vector>
#include <memory>
#include <functional>
#include "Vehicle.hpp"
#include "MC.hpp"
#include <threepp/threepp.hpp>

class VehicleManager {
public:
    VehicleManager() = default;

    void addVehicle(std::shared_ptr<threepp::Group> model, const VehicleStats& stats);

    bool switchVehicle(int index, threepp::Scene& scene, MC& mc);

    void updateCurrentVehicle(const threepp::Vector3& position,
                              const threepp::Vector3& rotation,
                              float lean);

    int getCurrentVehicleIndex() const { return currentIndex_; }
    size_t getVehicleCount() const { return vehicles_.size(); }
    const Vehicle& getCurrentVehicle() const { return vehicles_[currentIndex_]; }

    void addInitialVehicleToScene(threepp::Scene& scene);

private:
    std::vector<Vehicle> vehicles_;
    int currentIndex_{0};
};


#endif //THREEPPGP_VEHICLEMANAGER_HPP